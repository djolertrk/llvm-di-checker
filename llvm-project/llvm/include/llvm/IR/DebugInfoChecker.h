//===- DebugInfoChecker.h - Check the Debug Info metadata preservation ----===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
///
/// \file Interface to the `-debug-info-checker` - Debug Info testing utility.
///
//===----------------------------------------------------------------------===//

#ifndef LLVM_TRANSFORM_UTILS_DEBUGINFOCHECKER_H
#define LLVM_TRANSFORM_UTILS_DEBUGINFOCHECKER_H

#include "llvm/ADT/DenseMap.h"
#include "llvm/ADT/MapVector.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/PassManager.h"

using DebugFnMap = llvm::DenseMap<llvm::StringRef, const llvm::DISubprogram *>;
using DebugInstMap = llvm::DenseMap<llvm::Instruction*, bool>;

/// Track the Debug Info Metadata information.
struct ModuleDebugInfo {
  // This maps the function's name and its disubprogram.
  DebugFnMap DIFunctions;
  // This maps the instruction and the info if it has !dbg attached.
  DebugInstMap DILocations;
};

/// Map pass names to a per-pass ModuleDebugInfo instance.
using DebugInfoPerPassMap = llvm::MapVector<llvm::StringRef, ModuleDebugInfo>;

llvm::ModulePass *createCollectDICheckerModulePass(
    llvm::StringRef NameOfWrappedPass,
    DebugInfoPerPassMap *DIPreservationMap);

llvm::ModulePass *createCheckDICheckerModulePass(
    llvm::StringRef NameOfWrappedPass,
    DebugInfoPerPassMap *DIPreservationMap,
    llvm::StringRef DICFilePath = "");

namespace llvm {

/// Collect debug information of a module before a pass.
///
/// \param M The module to collect debug information from.
/// \param Functions A range of functions to collect debug information from.
/// \param DIPreservationMap A map to collect the DI metadata.
/// \param Banner A prefix string to add to debug/error messages.
/// \param NameOfWrappedPass A name of a pass to add to debug/error messages.
bool collectDebugInfoMetadata(Module &M,
                              iterator_range<Module::iterator> Functions,
                              DebugInfoPerPassMap &DIPreservationMap,
                              StringRef Banner,
                              StringRef NameOfWrappedPass);

/// Check debug information of a module after a pass.
///
/// \param M The module to collect debug information from.
/// \param Functions A range of functions to collect debug information from.
/// \param DIPreservationMap A map used to check collected the DI metadata.
/// \param Banner A prefix string to add to debug/error messages.
/// \param NameOfWrappedPass A name of a pass to add to debug/error messages.
bool checkDebugInfoMetadata(Module &M,
                            iterator_range<Module::iterator> Functions,
                            DebugInfoPerPassMap &DIPreservationMap,
                            StringRef Banner,
                            StringRef NameOfWrappedPass,
                            StringRef DICFilePath);

} // namespace llvm

#endif // LLVM_TRANSFORM_UTILS_DEBUGINFOCHECKER_H
