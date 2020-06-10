//===- DebugInfoChecker.cpp - Check the Debug Info metadata preservation --===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
///
/// \file Test the Debug Info (-g generated) preservation.
///
//===----------------------------------------------------------------------===//

#include "llvm/IR/DebugInfoChecker.h"
#include "llvm/IR/DebugInfo.h"
#include "llvm/IR/InstIterator.h"
#include "llvm/IR/IntrinsicInst.h"
#include "llvm/IR/Module.h"
#include "llvm/Pass.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/JSON.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

#define DEBUG_TYPE "di-checker"

static cl::opt<std::string>
    DICheckerExport("di-checker-export",
                    cl::desc("Export per-pass di-checker failures to "
                             "this (JSON) file (should be abs path)."),
                    cl::value_desc("filename"), cl::init(""));

namespace {
bool isFunctionSkipped(Function &F) {
  return F.isDeclaration() || !F.hasExactDefinition();
}
} // end anonymous namespace

bool llvm::collectDebugInfoMetadata(Module &M,
                                    iterator_range<Module::iterator> Functions,
                                    DebugInfoPerPassMap &DIPreservationMap,
                                    StringRef Banner,
                                    StringRef NameOfWrappedPass) {
  LLVM_DEBUG(dbgs() << Banner << " (before) " << NameOfWrappedPass << '\n');

  // Clear the map with the debug info before every single pass.
  DIPreservationMap.clear();

  if (!M.getNamedMetadata("llvm.dbg.cu")) {
    errs() << Banner << "(pass: " << NameOfWrappedPass << ")"
           << ": Skipping module without debug info\n";
    return false;
  }

  // Visit each instruction.
  for (Function &F : Functions) {
    if (isFunctionSkipped(F))
      continue;

    // Collect the DISubprogram.
    if (auto *SP = F.getSubprogram()) {
      LLVM_DEBUG(dbgs() << "  Collecting subprogram: " << *SP << '\n');
      DIPreservationMap[NameOfWrappedPass].DIFunctions.insert(
          {F.getName(), SP});
    } else {
      LLVM_DEBUG(dbgs() << "  Collecting subprogram: nullptr\n");
      DIPreservationMap[NameOfWrappedPass].DIFunctions.insert(
          {F.getName(), nullptr});
    }

    for (BasicBlock &BB : F) {
      // Collect debug locations (!dbg) and dbg.values.
      for (Instruction &I : BB) {
        // Skip PHIs.
        if (isa<PHINode>(I))
          continue;

        // Skip debug instructions.
        if (isa<DbgInfoIntrinsic>(&I))
          continue;

        LLVM_DEBUG(dbgs() << "  Collecting info for inst: " << I << '\n');

        const DILocation *Loc = I.getDebugLoc().get();
        bool HasLoc = Loc != nullptr;

        DIPreservationMap[NameOfWrappedPass].DILocations.insert(
            {&I, HasLoc});
      }
    }
  }

  return true;
}

// This checks the preservation of the dbg info attached to functions.
static bool checkFunctions(DebugFnMap &DIFunctionsBefore,
                           DebugFnMap &DIFunctionsAfter,
                           StringRef NameOfWrappedPass,
                           StringRef FileNameFromCU,
                           bool shouldWriteIntoJSON,
                           llvm::json::Array &Bugs) {
  bool Result = true;
  for (const auto &diFn : DIFunctionsAfter) {
    if (!diFn.second) {
      auto spIt = DIFunctionsBefore.find(diFn.first);
      if (spIt == DIFunctionsBefore.end()) {
        if (shouldWriteIntoJSON) {
          Bugs.push_back(llvm::json::Object(
               {{"metadata", "DISubprogram"},
                {"name", diFn.first}, {"action", "not-generate"}}));
        } else {
            errs() << "  ***ERROR: " << NameOfWrappedPass
               << " did not generate DISubprogram for the "
               << diFn.first
               <<  " from file: " << FileNameFromCU << '\n';
        }
        Result = false;
      } else {
        auto sp = spIt->second;
        // If the function had the SP attacched before the pass, consider it as
        // a debug info bug.
        if (sp) {
          if (shouldWriteIntoJSON) {
            Bugs.push_back(llvm::json::Object(
                 {{"metadata", "DISubprogram"},
                  {"name", diFn.first}, {"action", "drop"}}));
          } else {
            errs() << "  ***ERROR: " << NameOfWrappedPass
                 << " dropped DISubprogram for the "
                 << diFn.first
                 <<  " from file: " << FileNameFromCU << '\n';
          }
          Result = false;
        }
      }
    }
  }

  return Result;
}

// This checks the preservation of the dbg info attached to insts.
static bool checkInstructions(DebugInstMap &DILocsBefore,
                              DebugInstMap &DILocsAfter,
                              StringRef NameOfWrappedPass,
                              StringRef FileNameFromCU,
                              bool shouldWriteIntoJSON,
                              llvm::json::Array &Bugs) {
  bool Result = true;
  for (const auto &diLoc : DILocsAfter) {
    if (!diLoc.second) {
      auto Inst = diLoc.first;
      auto fnName = Inst->getFunction()->getName();
      auto BB = Inst->getParent();
      auto bbName = BB->hasName() ? BB->getName() : "no-name";

      auto instName = Instruction::getOpcodeName(Inst->getOpcode());
      auto instIt = DILocsBefore.find(Inst);
      if (instIt == DILocsBefore.end()) {
        if (shouldWriteIntoJSON) {
           Bugs.push_back(llvm::json::Object(
               {{"metadata", "DILocation"},
                {"fn-name", fnName.str()},
                {"bb-name", bbName.str()},
                {"instr", instName},
                {"action", "not-generate"}}));
        } else {
          errs() << "  ***ERROR: " << NameOfWrappedPass
                 << " did not generate DILocation for the "
                 << *Inst
                 << " (BB name: " << bbName << ")"
                 <<  " from file: " << FileNameFromCU << '\n';
        }
        Result = false;
      } else {
        auto hadLoc = instIt->second;
        // If the instr had the !dbg attacched before the pass, consider it as
        // a debug info bug.
        if (hadLoc) {
          if (shouldWriteIntoJSON) {
             Bugs.push_back(llvm::json::Object(
                 {{"metadata", "DILocation"},
                  {"fn-name", fnName.str()},
                  {"bb-name", bbName.str()},
                  {"instr", instName},
                  {"action", "drop"}}));
          } else {
            errs() << "  ***ERROR: " << NameOfWrappedPass
                   << " dropped DILocation for the "
                   << *Inst
                   << " (BB name: " << bbName << ")"
                   <<  " from file: " << FileNameFromCU << '\n';
          }
          Result = false;
        }
      }
    }
  }

  return Result;
}

// Write the json data into the file.
static void WriteJSON(StringRef DICFilePath,
                      StringRef FileNameFromCU,
                      StringRef NameOfWrappedPass,
                      llvm::json::Array &Bugs) {
  std::error_code EC;
  raw_fd_ostream OS_FILE{DICFilePath, EC,
                         sys::fs::OF_Append | sys::fs::OF_Text};
  if (EC) {
    errs() << "Could not open file: " << EC.message() << ", " << DICFilePath
           << '\n';
    return;
  }

  OS_FILE << "{\"file\":\"" << FileNameFromCU << "\", ";
  OS_FILE << "\"pass\":\"" << NameOfWrappedPass << "\", ";

  llvm::json::Value bugsToPrint {std::move(Bugs)};
  OS_FILE << "\"bugs\": " << bugsToPrint;

  OS_FILE << "}\n";
}

bool llvm::checkDebugInfoMetadata(Module &M,
                                  iterator_range<Module::iterator> Functions,
                                  DebugInfoPerPassMap &DIPreservationMap,
                                  StringRef Banner, StringRef NameOfWrappedPass,
                                  StringRef DICFilePath) {
  LLVM_DEBUG(dbgs() << Banner << " (after) " << NameOfWrappedPass << '\n');

  if (!M.getNamedMetadata("llvm.dbg.cu")) {
    errs() << Banner << "(pass: " << NameOfWrappedPass << ")"
           << ": Skipping module without debug info\n";
    return false;
  }

  // Map the debug info holding DIs after a pass.
  DebugInfoPerPassMap DIPreservationAfter;

  // Visit each instruction.
  for (Function &F : Functions) {
    if (isFunctionSkipped(F))
      continue;

    // Collect the DISubprogram.
    if (auto *SP = F.getSubprogram()) {
      LLVM_DEBUG(dbgs() << "  Collecting subprogram: " << *SP << '\n');
      DIPreservationAfter[NameOfWrappedPass].DIFunctions.insert(
          {F.getName(), SP});
    } else {
      LLVM_DEBUG(dbgs() << "  Collecting subprogram: nullptr\n");
      DIPreservationAfter[NameOfWrappedPass].DIFunctions.insert(
          {F.getName(), nullptr});
    }

    for (BasicBlock &BB : F) {
      // Collect debug locations (!dbg) and dbg.values.
      for (Instruction &I : BB) {
        // Skip PHIs.
        if (isa<PHINode>(I))
          continue;

        // Skip debug instructions.
        if (isa<DbgInfoIntrinsic>(&I))
          continue;

        LLVM_DEBUG(dbgs() << "  Collecting info for inst: " << I << '\n');

        const DILocation *Loc = I.getDebugLoc().get();
        bool HasLoc = Loc != nullptr;

        DIPreservationAfter[NameOfWrappedPass].DILocations.insert(
            {&I, HasLoc});
      }
    }
  }

  // TODO: The name of the module could be read better?
  StringRef FileNameFromCU =
      (cast<DICompileUnit>(M.getNamedMetadata("llvm.dbg.cu")->getOperand(0)))
          ->getFilename();

  auto DIFunctionsBefore = DIPreservationMap[NameOfWrappedPass].DIFunctions;
  auto DIFunctionsAfter = DIPreservationAfter[NameOfWrappedPass].DIFunctions;

  auto DILocsBefore = DIPreservationMap[NameOfWrappedPass].DILocations;
  auto DILocsAfter = DIPreservationAfter[NameOfWrappedPass].DILocations;

  bool shouldWriteIntoJSON = !DICFilePath.empty();
  llvm::json::Array Bugs;

  // TODO: Optimize this: It should itereate through
  //       the DIPreservationAfter[NameOfWrappedPass] only once.
  bool Result = checkFunctions(DIFunctionsBefore, DIFunctionsAfter,
                               NameOfWrappedPass, FileNameFromCU,
                               shouldWriteIntoJSON, Bugs) &&
                checkInstructions(DILocsBefore, DILocsAfter,
                                  NameOfWrappedPass, FileNameFromCU,
                                  shouldWriteIntoJSON, Bugs);

  if (shouldWriteIntoJSON && !Bugs.empty())
    WriteJSON(DICFilePath, FileNameFromCU, NameOfWrappedPass, Bugs);

  if (Result)
    errs() << NameOfWrappedPass << ": PASS\n";
  else
    errs() << NameOfWrappedPass << ": FAIL\n";

  LLVM_DEBUG(dbgs() << "\n\n");
  return Result;
}

/// ModulePass for collecting debug info form a module, used with the
/// legacy module pass manager.
struct CollectDICheckerModulePass : public ModulePass {
  bool runOnModule(Module &M) override {
    return collectDebugInfoMetadata(M, M.functions(), *DIPreservationMap,
                                    "CollectDIChecker: ", NameOfWrappedPass);
  }

  CollectDICheckerModulePass(StringRef NameOfWrappedPass = "",
                             DebugInfoPerPassMap *DIPreservationMap = nullptr)
      : ModulePass(ID), NameOfWrappedPass(NameOfWrappedPass),
        DIPreservationMap(DIPreservationMap) {}

  void getAnalysisUsage(AnalysisUsage &AU) const override {
    AU.setPreservesAll();
  }

  static char ID; // Pass identification.

private:
  StringRef NameOfWrappedPass;
  DebugInfoPerPassMap *DIPreservationMap;
};

/// ModulePass for checking debug info form a module, used with the
/// legacy module pass manager.
struct CheckDICheckerModulePass : public ModulePass {
  bool runOnModule(Module &M) override {
    if (DICFilePath.empty())
      DICFilePath = DICheckerExport;
    return checkDebugInfoMetadata(M, M.functions(), *DIPreservationMap,
                                  "CheckDIChecker: ", NameOfWrappedPass,
                                  DICFilePath);
  }

  CheckDICheckerModulePass(StringRef NameOfWrappedPass = "",
                           DebugInfoPerPassMap *DIPreservationMap = nullptr,
                           llvm::StringRef DICFilePath = "")
      : ModulePass(ID), NameOfWrappedPass(NameOfWrappedPass),
        DICFilePath(DICFilePath), DIPreservationMap(DIPreservationMap) {}

  void getAnalysisUsage(AnalysisUsage &AU) const override {
    AU.setPreservesAll();
  }

  static char ID; // Pass identification.

private:
  StringRef NameOfWrappedPass;
  StringRef DICFilePath;
  DebugInfoPerPassMap *DIPreservationMap;
};

ModulePass *
createCollectDICheckerModulePass(llvm::StringRef NameOfWrappedPass,
                                 DebugInfoPerPassMap *DIPreservationMap) {
  return new CollectDICheckerModulePass(NameOfWrappedPass, DIPreservationMap);
}

ModulePass *
createCheckDICheckerModulePass(llvm::StringRef NameOfWrappedPass,
                               DebugInfoPerPassMap *DIPreservationMap,
                               llvm::StringRef DICFilePath) {
  return new CheckDICheckerModulePass(NameOfWrappedPass, DIPreservationMap,
                                      DICFilePath);
}

char CollectDICheckerModulePass::ID = 0;
static RegisterPass<CollectDICheckerModulePass>
    DICCollect("debug-info-checker-collect",
               "Collecct the debug info for the pass preservation checking");

char CheckDICheckerModulePass::ID = 0;
static RegisterPass<CheckDICheckerModulePass>
    DICCheck("debug-info-checker-check", "Check the debug info after a pass");
