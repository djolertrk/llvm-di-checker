; RUN: opt -O2 -di-checker -S -o - < %s 2>&1 | FileCheck %s

; RUN: rm -rf %t.json
; RUN: opt -O2 -di-checker -di-checker-export=%t.json -S -o - < %s
; RUN: cat %t.json | FileCheck %s -check-prefix=CHECK-JSON

; CHECK: Force set function attributes: PASS
; CHECK: Infer set function attributes: PASS
; CHECK: Interprocedural Sparse Conditional Constant Propagation: PASS
; CHECK: Called Value Propagation: PASS
; CHECK: Global Variable Optimizer: PASS
; CHECK: Promote Memory to Register: PASS
; CHECK: Dead Argument Elimination: PASS
; CHECK: Combine redundant instructions: PASS
; CHECK: Simplify the CFG: PASS
; CHECK: Globals Alias Analysis: PASS
; CHECK: Remove unused exception handling info: PASS
; CHECK: Function Integration/Inlining: PASS
; CHECK: OpenMP specific optimizations: PASS
; CHECK: Deduce function attributes: PASS
; CHECK: SROA: PASS
; CHECK: Early CSE w/ MemorySSA: PASS
; CHECK: Speculatively execute instructions if target has divergent branches: PASS
; CHECK: Jump Threading: PASS
; CHECK: Value Propagation: PASS
; CHECK: Simplify the CFG: PASS
; CHECK: ***ERROR: Combine redundant instructions dropped DILocation for the
; CHECK: ***ERROR: Combine redundant instructions dropped DILocation for the
; CHECK: Combine redundant instructions: FAIL
; CHECK: Conditionally eliminate dead library calls: PASS
; CHECK: PGOMemOPSize: PASS
; CHECK: Tail Call Elimination: PASS
; CHECK: Simplify the CFG: PASS
; CHECK: Reassociate expressions: PASS
; CHECK: Rotate Loops: PASS
; CHECK: Loop Invariant Code Motion: PASS
; CHECK: Unswitch loops: PASS
; CHECK: Simplify the CFG: PASS
; CHECK: Combine redundant instructions: PASS
; CHECK: Induction Variable Simplification: PASS
; CHECK: Recognize loop idioms: PASS
; CHECK: Delete dead loops: PASS
; CHECK: Unroll loops: PASS
; CHECK: MergedLoadStoreMotion: PASS
; CHECK: Global Value Numbering: PASS
; CHECK: MemCpy Optimization: PASS
; CHECK: Sparse Conditional Constant Propagation: PASS
; CHECK: Bit-Tracking Dead Code Elimination: PASS
; CHECK: Combine redundant instructions: PASS
; CHECK: Jump Threading: PASS
; CHECK: Value Propagation: PASS
; CHECK: Dead Store Elimination: PASS
; CHECK: Loop Invariant Code Motion: PASS
; CHECK: Aggressive Dead Code Elimination: PASS
; CHECK: Simplify the CFG: PASS
; CHECK: Combine redundant instructions: PASS
; CHECK: A No-Op Barrier Pass: PASS
; CHECK: Eliminate Available Externally Globals: PASS
; CHECK: Deduce function attributes in RPO: PASS
; CHECK: Global Variable Optimizer: PASS
; CHECK: Dead Global Elimination: PASS
; CHECK: Globals Alias Analysis: PASS
; CHECK: Float to int: PASS
; CHECK: Lower constant intrinsics: PASS
; CHECK: Rotate Loops: PASS
; CHECK: Loop Distribution: PASS
; CHECK: Loop Vectorization: PASS
; CHECK: Loop Load Elimination: PASS
; CHECK: Combine redundant instructions: PASS
; CHECK: Simplify the CFG: PASS
; CHECK: SLP Vectorizer: PASS
; CHECK: Optimize scalar/vector ops: PASS
; CHECK: Combine redundant instructions: PASS
; CHECK: Unroll loops: PASS
; CHECK: Combine redundant instructions: PASS
; CHECK: Loop Invariant Code Motion: PASS
; CHECK: Warn about non-applied transformations: PASS
; CHECK: Alignment from assumptions: PASS
; CHECK: Strip Unused Function Prototypes: PASS
; CHECK: Dead Global Elimination: PASS
; CHECK: Merge Duplicate Global Constants: PASS
; CHECK: Loop Sink: PASS
; CHECK: Remove redundant instructions: PASS
; CHECK: Hoist/decompose integer division and remainder: PASS
; CHECK: Simplify the CFG: PASS
; CHECK: Module Verifier: PASS

; CHECK-JSON: {"file":"simple.c", "pass":"Combine redundant instructions", "bugs": {{.*}}

; ModuleID = 'simple.c'
source_filename = "simple.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: nounwind uwtable
define dso_local i32 @fn1(i32 %arg1) #0 !dbg !12 {
entry:
  %retval = alloca i32, align 4
  %arg1.addr = alloca i32, align 4
  store i32 %arg1, i32* %arg1.addr, align 4, !tbaa !17
  call void @llvm.dbg.declare(metadata i32* %arg1.addr, metadata !16, metadata !DIExpression()), !dbg !21
  %0 = load i32, i32* %arg1.addr, align 4, !dbg !22, !tbaa !17
  %rem = srem i32 %0, 2, !dbg !24
  %tobool = icmp ne i32 %rem, 0, !dbg !24
  br i1 %tobool, label %if.then, label %if.end, !dbg !25

if.then:                                          ; preds = %entry
  %1 = load i32, i32* %arg1.addr, align 4, !dbg !26, !tbaa !17
  %inc = add nsw i32 %1, 1, !dbg !26
  store i32 %inc, i32* %arg1.addr, align 4, !dbg !26, !tbaa !17
  store i32 %inc, i32* %retval, align 4, !dbg !27
  br label %return, !dbg !27

if.end:                                           ; preds = %entry
  %2 = load i32, i32* %arg1.addr, align 4, !dbg !28, !tbaa !17
  store i32 %2, i32* %retval, align 4, !dbg !29
  br label %return, !dbg !29

return:                                           ; preds = %if.end, %if.then
  %3 = load i32, i32* %retval, align 4, !dbg !30
  ret i32 %3, !dbg !30
}

; Function Attrs: nounwind readnone speculatable willreturn
declare void @llvm.dbg.declare(metadata, metadata, metadata) #1

; Function Attrs: nounwind uwtable
define dso_local i32 @fn2() #0 !dbg !31 {
entry:
  %local1 = alloca i32, align 4
  %0 = bitcast i32* %local1 to i8*, !dbg !36
  call void @llvm.lifetime.start.p0i8(i64 4, i8* %0) #4, !dbg !36
  call void @llvm.dbg.declare(metadata i32* %local1, metadata !35, metadata !DIExpression()), !dbg !37
  %call = call i32 (...) @fn3(), !dbg !38
  store i32 %call, i32* %local1, align 4, !dbg !37, !tbaa !17
  %1 = load i32, i32* %local1, align 4, !dbg !39, !tbaa !17
  %call1 = call i32 @fn1(i32 %1), !dbg !40
  %2 = load i32, i32* %local1, align 4, !tbaa !17
  %add = add nsw i32 %2, %call1
  store i32 %add, i32* %local1, align 4, !tbaa !17
  %3 = load i32, i32* %local1, align 4, !dbg !42, !tbaa !17
  %4 = bitcast i32* %local1 to i8*, !dbg !43
  call void @llvm.lifetime.end.p0i8(i64 4, i8* %4) #4, !dbg !43
  ret i32 %3, !dbg !44
}

; Function Attrs: argmemonly nounwind willreturn
declare void @llvm.lifetime.start.p0i8(i64 immarg, i8* nocapture) #2

declare !dbg !4 dso_local i32 @fn3(...) #3

; Function Attrs: argmemonly nounwind willreturn
declare void @llvm.lifetime.end.p0i8(i64 immarg, i8* nocapture) #2

attributes #0 = { nounwind uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "frame-pointer"="none" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind readnone speculatable willreturn }
attributes #2 = { argmemonly nounwind willreturn }
attributes #3 = { "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "frame-pointer"="none" "less-precise-fpmad"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #4 = { nounwind }

!llvm.dbg.cu = !{!0}
!llvm.module.flags = !{!8, !9, !10}
!llvm.ident = !{!11}

!0 = distinct !DICompileUnit(language: DW_LANG_C99, file: !1, producer: "clang version 11.0.0", isOptimized: true, runtimeVersion: 0, emissionKind: FullDebug, enums: !2, retainedTypes: !3, splitDebugInlining: false, nameTableKind: None)
!1 = !DIFile(filename: "simple.c", directory: "/dir")
!2 = !{}
!3 = !{!4}
!4 = !DISubprogram(name: "fn3", scope: !1, file: !1, line: 1, type: !5, spFlags: DISPFlagOptimized, retainedNodes: !2)
!5 = !DISubroutineType(types: !6)
!6 = !{!7, null}
!7 = !DIBasicType(name: "int", size: 32, encoding: DW_ATE_signed)
!8 = !{i32 7, !"Dwarf Version", i32 4}
!9 = !{i32 2, !"Debug Info Version", i32 3}
!10 = !{i32 1, !"wchar_size", i32 4}
!11 = !{!"clang version 11.0.0"}
!12 = distinct !DISubprogram(name: "fn1", scope: !1, file: !1, line: 3, type: !13, scopeLine: 3, flags: DIFlagPrototyped | DIFlagAllCallsDescribed, spFlags: DISPFlagDefinition | DISPFlagOptimized, unit: !0, retainedNodes: !15)
!13 = !DISubroutineType(types: !14)
!14 = !{!7, !7}
!15 = !{!16}
!16 = !DILocalVariable(name: "arg1", arg: 1, scope: !12, file: !1, line: 3, type: !7)
!17 = !{!18, !18, i64 0}
!18 = !{!"int", !19, i64 0}
!19 = !{!"omnipotent char", !20, i64 0}
!20 = !{!"Simple C/C++ TBAA"}
!21 = !DILocation(line: 3, column: 13, scope: !12)
!22 = !DILocation(line: 4, column: 7, scope: !23)
!23 = distinct !DILexicalBlock(scope: !12, file: !1, line: 4, column: 7)
!24 = !DILocation(line: 4, column: 12, scope: !23)
!25 = !DILocation(line: 4, column: 7, scope: !12)
!26 = !DILocation(line: 5, column: 12, scope: !23)
!27 = !DILocation(line: 5, column: 5, scope: !23)
!28 = !DILocation(line: 7, column: 10, scope: !12)
!29 = !DILocation(line: 7, column: 3, scope: !12)
!30 = !DILocation(line: 8, column: 1, scope: !12)
!31 = distinct !DISubprogram(name: "fn2", scope: !1, file: !1, line: 10, type: !32, scopeLine: 10, flags: DIFlagAllCallsDescribed, spFlags: DISPFlagDefinition | DISPFlagOptimized, unit: !0, retainedNodes: !34)
!32 = !DISubroutineType(types: !33)
!33 = !{!7}
!34 = !{!35}
!35 = !DILocalVariable(name: "local1", scope: !31, file: !1, line: 11, type: !7)
!36 = !DILocation(line: 11, column: 3, scope: !31)
!37 = !DILocation(line: 11, column: 7, scope: !31)
!38 = !DILocation(line: 11, column: 16, scope: !31)
!39 = !DILocation(line: 12, column: 17, scope: !31)
!40 = !DILocation(line: 12, column: 13, scope: !31)
!42 = !DILocation(line: 14, column: 10, scope: !31)
!43 = !DILocation(line: 15, column: 1, scope: !31)
!44 = !DILocation(line: 14, column: 3, scope: !31)
