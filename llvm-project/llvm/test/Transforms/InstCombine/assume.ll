; NOTE: Assertions have been autogenerated by utils/update_test_checks.py
; RUN: opt < %s -instcombine -S | FileCheck %s
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define i32 @foo1(i32* %a) #0 {
; CHECK-LABEL: @foo1(
; CHECK-NEXT:  entry:
; CHECK-NEXT:    [[TMP0:%.*]] = load i32, i32* [[A:%.*]], align 32
; CHECK-NEXT:    [[PTRINT:%.*]] = ptrtoint i32* [[A]] to i64
; CHECK-NEXT:    [[MASKEDPTR:%.*]] = and i64 [[PTRINT]], 31
; CHECK-NEXT:    [[MASKCOND:%.*]] = icmp eq i64 [[MASKEDPTR]], 0
; CHECK-NEXT:    tail call void @llvm.assume(i1 [[MASKCOND]])
; CHECK-NEXT:    ret i32 [[TMP0]]
;
entry:
  %0 = load i32, i32* %a, align 4

; Check that the alignment has been upgraded and that the assume has not
; been removed:

  %ptrint = ptrtoint i32* %a to i64
  %maskedptr = and i64 %ptrint, 31
  %maskcond = icmp eq i64 %maskedptr, 0
  tail call void @llvm.assume(i1 %maskcond)

  ret i32 %0
}

define i32 @foo2(i32* %a) #0 {
; CHECK-LABEL: @foo2(
; CHECK-NEXT:  entry:
; CHECK-NEXT:    [[PTRINT:%.*]] = ptrtoint i32* [[A:%.*]] to i64
; CHECK-NEXT:    [[MASKEDPTR:%.*]] = and i64 [[PTRINT]], 31
; CHECK-NEXT:    [[MASKCOND:%.*]] = icmp eq i64 [[MASKEDPTR]], 0
; CHECK-NEXT:    tail call void @llvm.assume(i1 [[MASKCOND]])
; CHECK-NEXT:    [[TMP0:%.*]] = load i32, i32* [[A]], align 32
; CHECK-NEXT:    ret i32 [[TMP0]]
;
entry:
; Same check as in @foo1, but make sure it works if the assume is first too.

  %ptrint = ptrtoint i32* %a to i64
  %maskedptr = and i64 %ptrint, 31
  %maskcond = icmp eq i64 %maskedptr, 0
  tail call void @llvm.assume(i1 %maskcond)

  %0 = load i32, i32* %a, align 4
  ret i32 %0
}

declare void @llvm.assume(i1) #1

define i32 @simple(i32 %a) #1 {
; CHECK-LABEL: @simple(
; CHECK-NEXT:  entry:
; CHECK-NEXT:    [[CMP:%.*]] = icmp eq i32 [[A:%.*]], 4
; CHECK-NEXT:    tail call void @llvm.assume(i1 [[CMP]])
; CHECK-NEXT:    ret i32 4
;
entry:


  %cmp = icmp eq i32 %a, 4
  tail call void @llvm.assume(i1 %cmp)
  ret i32 %a
}

define i32 @can1(i1 %a, i1 %b, i1 %c) {
; CHECK-LABEL: @can1(
; CHECK-NEXT:  entry:
; CHECK-NEXT:    call void @llvm.assume(i1 [[A:%.*]])
; CHECK-NEXT:    call void @llvm.assume(i1 [[B:%.*]])
; CHECK-NEXT:    call void @llvm.assume(i1 [[C:%.*]])
; CHECK-NEXT:    ret i32 5
;
entry:
  %and1 = and i1 %a, %b
  %and  = and i1 %and1, %c
  tail call void @llvm.assume(i1 %and)


  ret i32 5
}

define i32 @can2(i1 %a, i1 %b, i1 %c) {
; CHECK-LABEL: @can2(
; CHECK-NEXT:  entry:
; CHECK-NEXT:    [[TMP0:%.*]] = xor i1 [[A:%.*]], true
; CHECK-NEXT:    call void @llvm.assume(i1 [[TMP0]])
; CHECK-NEXT:    [[TMP1:%.*]] = xor i1 [[B:%.*]], true
; CHECK-NEXT:    call void @llvm.assume(i1 [[TMP1]])
; CHECK-NEXT:    ret i32 5
;
entry:
  %v = or i1 %a, %b
  %w = xor i1 %v, 1
  tail call void @llvm.assume(i1 %w)


  ret i32 5
}

define i32 @bar1(i32 %a) #0 {
; CHECK-LABEL: @bar1(
; CHECK-NEXT:  entry:
; CHECK-NEXT:    [[AND:%.*]] = and i32 [[A:%.*]], 7
; CHECK-NEXT:    [[CMP:%.*]] = icmp eq i32 [[AND]], 1
; CHECK-NEXT:    tail call void @llvm.assume(i1 [[CMP]])
; CHECK-NEXT:    ret i32 1
;
entry:
  %and1 = and i32 %a, 3


  %and = and i32 %a, 7
  %cmp = icmp eq i32 %and, 1
  tail call void @llvm.assume(i1 %cmp)

  ret i32 %and1
}

define i32 @bar2(i32 %a) #0 {
; CHECK-LABEL: @bar2(
; CHECK-NEXT:  entry:
; CHECK-NEXT:    [[AND:%.*]] = and i32 [[A:%.*]], 7
; CHECK-NEXT:    [[CMP:%.*]] = icmp eq i32 [[AND]], 1
; CHECK-NEXT:    tail call void @llvm.assume(i1 [[CMP]])
; CHECK-NEXT:    ret i32 1
;
entry:

  %and = and i32 %a, 7
  %cmp = icmp eq i32 %and, 1
  tail call void @llvm.assume(i1 %cmp)

  %and1 = and i32 %a, 3
  ret i32 %and1
}

define i32 @bar3(i32 %a, i1 %x, i1 %y) #0 {
; CHECK-LABEL: @bar3(
; CHECK-NEXT:  entry:
; CHECK-NEXT:    tail call void @llvm.assume(i1 [[X:%.*]])
; CHECK-NEXT:    [[AND:%.*]] = and i32 [[A:%.*]], 7
; CHECK-NEXT:    [[CMP:%.*]] = icmp eq i32 [[AND]], 1
; CHECK-NEXT:    tail call void @llvm.assume(i1 [[CMP]])
; CHECK-NEXT:    tail call void @llvm.assume(i1 [[Y:%.*]])
; CHECK-NEXT:    ret i32 1
;
entry:
  %and1 = and i32 %a, 3

; Don't be fooled by other assumes around.

  tail call void @llvm.assume(i1 %x)

  %and = and i32 %a, 7
  %cmp = icmp eq i32 %and, 1
  tail call void @llvm.assume(i1 %cmp)

  tail call void @llvm.assume(i1 %y)

  ret i32 %and1
}

define i32 @bar4(i32 %a, i32 %b) {
; CHECK-LABEL: @bar4(
; CHECK-NEXT:  entry:
; CHECK-NEXT:    [[AND:%.*]] = and i32 [[A:%.*]], 7
; CHECK-NEXT:    [[CMP:%.*]] = icmp eq i32 [[AND]], 1
; CHECK-NEXT:    tail call void @llvm.assume(i1 [[CMP]])
; CHECK-NEXT:    [[CMP2:%.*]] = icmp eq i32 [[A]], [[B:%.*]]
; CHECK-NEXT:    tail call void @llvm.assume(i1 [[CMP2]])
; CHECK-NEXT:    ret i32 1
;
entry:
  %and1 = and i32 %b, 3


  %and = and i32 %a, 7
  %cmp = icmp eq i32 %and, 1
  tail call void @llvm.assume(i1 %cmp)

  %cmp2 = icmp eq i32 %a, %b
  tail call void @llvm.assume(i1 %cmp2)

  ret i32 %and1
}

define i32 @icmp1(i32 %a) #0 {
; CHECK-LABEL: @icmp1(
; CHECK-NEXT:    [[CMP:%.*]] = icmp sgt i32 [[A:%.*]], 5
; CHECK-NEXT:    tail call void @llvm.assume(i1 [[CMP]])
; CHECK-NEXT:    ret i32 1
;
  %cmp = icmp sgt i32 %a, 5
  tail call void @llvm.assume(i1 %cmp)
  %conv = zext i1 %cmp to i32
  ret i32 %conv
}

define i32 @icmp2(i32 %a) #0 {
; CHECK-LABEL: @icmp2(
; CHECK-NEXT:    [[CMP:%.*]] = icmp sgt i32 [[A:%.*]], 5
; CHECK-NEXT:    tail call void @llvm.assume(i1 [[CMP]])
; CHECK-NEXT:    ret i32 0
;
  %cmp = icmp sgt i32 %a, 5
  tail call void @llvm.assume(i1 %cmp)
  %t0 = zext i1 %cmp to i32
  %lnot.ext = xor i32 %t0, 1
  ret i32 %lnot.ext
}

; If the 'not' of a condition is known true, then the condition must be false.

define i1 @assume_not(i1 %cond) {
; CHECK-LABEL: @assume_not(
; CHECK-NEXT:    [[NOTCOND:%.*]] = xor i1 [[COND:%.*]], true
; CHECK-NEXT:    call void @llvm.assume(i1 [[NOTCOND]])
; CHECK-NEXT:    ret i1 false
;
  %notcond = xor i1 %cond, true
  call void @llvm.assume(i1 %notcond)
  ret i1 %cond
}

declare void @escape(i32* %a)

; Canonicalize a nonnull assumption on a load into metadata form.

define i32 @bundle1(i32* %P) {
; CHECK-LABEL: @bundle1(
; CHECK-NEXT:    tail call void @llvm.assume(i1 true) [ "nonnull"(i32* [[P:%.*]]) ]
; CHECK-NEXT:    [[LOAD:%.*]] = load i32, i32* [[P]], align 4
; CHECK-NEXT:    ret i32 [[LOAD]]
;
  tail call void @llvm.assume(i1 true) ["nonnull"(i32* %P)]
  %load = load i32, i32* %P
  ret i32 %load
}

define i32 @bundle2(i32* %P) {
; CHECK-LABEL: @bundle2(
; CHECK-NEXT:    [[LOAD:%.*]] = load i32, i32* [[P:%.*]], align 4
; CHECK-NEXT:    ret i32 [[LOAD]]
;
  tail call void @llvm.assume(i1 true) ["ignore"(i32* undef)]
  %load = load i32, i32* %P
  ret i32 %load
}

define i1 @nonnull1(i32** %a) {
; CHECK-LABEL: @nonnull1(
; CHECK-NEXT:    [[LOAD:%.*]] = load i32*, i32** [[A:%.*]], align 8, !nonnull !6
; CHECK-NEXT:    tail call void @escape(i32* nonnull [[LOAD]])
; CHECK-NEXT:    ret i1 false
;
  %load = load i32*, i32** %a
  %cmp = icmp ne i32* %load, null
  tail call void @llvm.assume(i1 %cmp)
  tail call void @escape(i32* %load)
  %rval = icmp eq i32* %load, null
  ret i1 %rval
}

; Make sure the above canonicalization applies only
; to pointer types.  Doing otherwise would be illegal.

define i1 @nonnull2(i32* %a) {
; CHECK-LABEL: @nonnull2(
; CHECK-NEXT:    [[LOAD:%.*]] = load i32, i32* [[A:%.*]], align 4
; CHECK-NEXT:    [[CMP:%.*]] = icmp ne i32 [[LOAD]], 0
; CHECK-NEXT:    tail call void @llvm.assume(i1 [[CMP]])
; CHECK-NEXT:    ret i1 false
;
  %load = load i32, i32* %a
  %cmp = icmp ne i32 %load, 0
  tail call void @llvm.assume(i1 %cmp)
  %rval = icmp eq i32 %load, 0
  ret i1 %rval
}

; Make sure the above canonicalization does not trigger
; if the assume is control dependent on something else

define i1 @nonnull3(i32** %a, i1 %control) {
; CHECK-LABEL: @nonnull3(
; CHECK-NEXT:  entry:
; CHECK-NEXT:    [[LOAD:%.*]] = load i32*, i32** [[A:%.*]], align 8
; CHECK-NEXT:    [[CMP:%.*]] = icmp ne i32* [[LOAD]], null
; CHECK-NEXT:    br i1 [[CONTROL:%.*]], label [[TAKEN:%.*]], label [[NOT_TAKEN:%.*]]
; CHECK:       taken:
; CHECK-NEXT:    tail call void @llvm.assume(i1 [[CMP]])
; CHECK-NEXT:    ret i1 false
; CHECK:       not_taken:
; CHECK-NEXT:    [[RVAL_2:%.*]] = icmp sgt i32* [[LOAD]], null
; CHECK-NEXT:    ret i1 [[RVAL_2]]
;
entry:
  %load = load i32*, i32** %a
  %cmp = icmp ne i32* %load, null
  br i1 %control, label %taken, label %not_taken
taken:
  tail call void @llvm.assume(i1 %cmp)
  %rval = icmp eq i32* %load, null
  ret i1 %rval
not_taken:
  %rval.2 = icmp sgt i32* %load, null
  ret i1 %rval.2
}

; Make sure the above canonicalization does not trigger
; if the path from the load to the assume is potentially
; interrupted by an exception being thrown

define i1 @nonnull4(i32** %a) {
; CHECK-LABEL: @nonnull4(
; CHECK-NEXT:    [[LOAD:%.*]] = load i32*, i32** [[A:%.*]], align 8
; CHECK-NEXT:    tail call void @escape(i32* [[LOAD]])
; CHECK-NEXT:    [[CMP:%.*]] = icmp ne i32* [[LOAD]], null
; CHECK-NEXT:    tail call void @llvm.assume(i1 [[CMP]])
; CHECK-NEXT:    ret i1 false
;
  %load = load i32*, i32** %a
  ;; This call may throw!
  tail call void @escape(i32* %load)
  %cmp = icmp ne i32* %load, null
  tail call void @llvm.assume(i1 %cmp)
  %rval = icmp eq i32* %load, null
  ret i1 %rval
}
define i1 @nonnull5(i32** %a) {
; CHECK-LABEL: @nonnull5(
; CHECK-NEXT:    [[LOAD:%.*]] = load i32*, i32** [[A:%.*]], align 8
; CHECK-NEXT:    tail call void @escape(i32* [[LOAD]])
; CHECK-NEXT:    [[CMP:%.*]] = icmp slt i32* [[LOAD]], null
; CHECK-NEXT:    tail call void @llvm.assume(i1 [[CMP]])
; CHECK-NEXT:    ret i1 false
;
  %load = load i32*, i32** %a
  ;; This call may throw!
  tail call void @escape(i32* %load)
  %integral = ptrtoint i32* %load to i64
  %cmp = icmp slt i64 %integral, 0
  tail call void @llvm.assume(i1 %cmp) ; %load has at least highest bit set
  %rval = icmp eq i32* %load, null
  ret i1 %rval
}

; PR35846 - https://bugs.llvm.org/show_bug.cgi?id=35846

define i32 @assumption_conflicts_with_known_bits(i32 %a, i32 %b) {
; CHECK-LABEL: @assumption_conflicts_with_known_bits(
; CHECK-NEXT:    [[AND1:%.*]] = and i32 [[B:%.*]], 3
; CHECK-NEXT:    tail call void @llvm.assume(i1 false)
; CHECK-NEXT:    [[CMP2:%.*]] = icmp eq i32 [[AND1]], 0
; CHECK-NEXT:    tail call void @llvm.assume(i1 [[CMP2]])
; CHECK-NEXT:    ret i32 0
;
  %and1 = and i32 %b, 3
  %B1 = lshr i32 %and1, %and1
  %B3 = shl nuw nsw i32 %and1, %B1
  %cmp = icmp eq i32 %B3, 1
  tail call void @llvm.assume(i1 %cmp)
  %cmp2 = icmp eq i32 %B1, %B3
  tail call void @llvm.assume(i1 %cmp2)
  ret i32 %and1
}

; PR37726 - https://bugs.llvm.org/show_bug.cgi?id=37726
; There's a loophole in eliminating a redundant assumption when
; we have conflicting assumptions. Verify that debuginfo doesn't
; get in the way of the fold.

define void @debug_interference(i8 %x) {
; CHECK-LABEL: @debug_interference(
; CHECK-NEXT:    [[CMP2:%.*]] = icmp ne i8 [[X:%.*]], 0
; CHECK-NEXT:    tail call void @llvm.dbg.value(metadata i32 5, metadata !7, metadata !DIExpression()), !dbg !9
; CHECK-NEXT:    tail call void @llvm.assume(i1 false)
; CHECK-NEXT:    tail call void @llvm.dbg.value(metadata i32 5, metadata !7, metadata !DIExpression()), !dbg !9
; CHECK-NEXT:    tail call void @llvm.dbg.value(metadata i32 5, metadata !7, metadata !DIExpression()), !dbg !9
; CHECK-NEXT:    tail call void @llvm.assume(i1 [[CMP2]])
; CHECK-NEXT:    ret void
;
  %cmp1 = icmp eq i8 %x, 0
  %cmp2 = icmp ne i8 %x, 0
  tail call void @llvm.assume(i1 %cmp1)
  tail call void @llvm.dbg.value(metadata i32 5, metadata !1, metadata !DIExpression()), !dbg !9
  tail call void @llvm.assume(i1 %cmp1)
  tail call void @llvm.dbg.value(metadata i32 5, metadata !1, metadata !DIExpression()), !dbg !9
  tail call void @llvm.assume(i1 %cmp2)
  tail call void @llvm.dbg.value(metadata i32 5, metadata !1, metadata !DIExpression()), !dbg !9
  tail call void @llvm.assume(i1 %cmp2)
  ret void
}

; This would crash.
; Does it ever make sense to peek through a bitcast of the icmp operand?

define i32 @PR40940(<4 x i8> %x) {
; CHECK-LABEL: @PR40940(
; CHECK-NEXT:    [[SHUF:%.*]] = shufflevector <4 x i8> [[X:%.*]], <4 x i8> undef, <4 x i32> <i32 1, i32 1, i32 2, i32 3>
; CHECK-NEXT:    [[T2:%.*]] = bitcast <4 x i8> [[SHUF]] to i32
; CHECK-NEXT:    [[T3:%.*]] = icmp ult i32 [[T2]], 65536
; CHECK-NEXT:    call void @llvm.assume(i1 [[T3]])
; CHECK-NEXT:    ret i32 [[T2]]
;
  %shuf = shufflevector <4 x i8> %x, <4 x i8> undef, <4 x i32> <i32 1, i32 1, i32 2, i32 3>
  %t2 = bitcast <4 x i8> %shuf to i32
  %t3 = icmp ult i32 %t2, 65536
  call void @llvm.assume(i1 %t3)
  ret i32 %t2
}

define i1 @nonnull3A(i32** %a, i1 %control) {
; CHECK-LABEL: @nonnull3A(
; CHECK-NEXT:  entry:
; CHECK-NEXT:    [[LOAD:%.*]] = load i32*, i32** [[A:%.*]], align 8
; CHECK-NEXT:    br i1 [[CONTROL:%.*]], label [[TAKEN:%.*]], label [[NOT_TAKEN:%.*]]
; CHECK:       taken:
; CHECK-NEXT:    [[CMP:%.*]] = icmp ne i32* [[LOAD]], null
; CHECK-NEXT:    call void @llvm.assume(i1 [[CMP]])
; CHECK-NEXT:    ret i1 true
; CHECK:       not_taken:
; CHECK-NEXT:    [[RVAL_2:%.*]] = icmp sgt i32* [[LOAD]], null
; CHECK-NEXT:    ret i1 [[RVAL_2]]
;
entry:
  %load = load i32*, i32** %a
  %cmp = icmp ne i32* %load, null
  br i1 %control, label %taken, label %not_taken
taken:
  call void @llvm.assume(i1 %cmp)
  ret i1 %cmp
not_taken:
  call void @llvm.assume(i1 %cmp)
  %rval.2 = icmp sgt i32* %load, null
  ret i1 %rval.2
}

define i1 @nonnull3B(i32** %a, i1 %control) {
; CHECK-LABEL: @nonnull3B(
; CHECK-NEXT:  entry:
; CHECK-NEXT:    br i1 [[CONTROL:%.*]], label [[TAKEN:%.*]], label [[NOT_TAKEN:%.*]]
; CHECK:       taken:
; CHECK-NEXT:    [[LOAD:%.*]] = load i32*, i32** [[A:%.*]], align 8
; CHECK-NEXT:    [[CMP:%.*]] = icmp ne i32* [[LOAD]], null
; CHECK-NEXT:    call void @llvm.assume(i1 [[CMP]]) [ "nonnull"(i32* [[LOAD]]), "nonnull"(i1 [[CMP]]) ]
; CHECK-NEXT:    ret i1 true
; CHECK:       not_taken:
; CHECK-NEXT:    ret i1 [[CONTROL]]
;
entry:
  %load = load i32*, i32** %a
  %cmp = icmp ne i32* %load, null
  br i1 %control, label %taken, label %not_taken
taken:
  call void @llvm.assume(i1 %cmp) ["nonnull"(i32* %load), "nonnull"(i1 %cmp)]
  ret i1 %cmp
not_taken:
  call void @llvm.assume(i1 %cmp) ["nonnull"(i32* %load), "nonnull"(i1 %cmp)]
  ret i1 %control
}

declare i1 @tmp1(i1)

define i1 @nonnull3C(i32** %a, i1 %control) {
; CHECK-LABEL: @nonnull3C(
; CHECK-NEXT:  entry:
; CHECK-NEXT:    br i1 [[CONTROL:%.*]], label [[TAKEN:%.*]], label [[NOT_TAKEN:%.*]]
; CHECK:       taken:
; CHECK-NEXT:    [[LOAD:%.*]] = load i32*, i32** [[A:%.*]], align 8
; CHECK-NEXT:    [[CMP:%.*]] = icmp ne i32* [[LOAD]], null
; CHECK-NEXT:    [[CMP2:%.*]] = call i1 @tmp1(i1 [[CMP]])
; CHECK-NEXT:    br label [[EXIT:%.*]]
; CHECK:       exit:
; CHECK-NEXT:    ret i1 [[CMP2]]
; CHECK:       not_taken:
; CHECK-NEXT:    ret i1 [[CONTROL]]
;
entry:
  %load = load i32*, i32** %a
  %cmp = icmp ne i32* %load, null
  br i1 %control, label %taken, label %not_taken
taken:
  %cmp2 = call i1 @tmp1(i1 %cmp)
  br label %exit
exit:
  ; FIXME: this shouldn't be dropped because it is still dominated by the new position of %load
  call void @llvm.assume(i1 %cmp) ["nonnull"(i32* %load), "nonnull"(i1 %cmp)]
  ret i1 %cmp2
not_taken:
  call void @llvm.assume(i1 %cmp)
  ret i1 %control
}

define i1 @nonnull3D(i32** %a, i1 %control) {
; CHECK-LABEL: @nonnull3D(
; CHECK-NEXT:  entry:
; CHECK-NEXT:    br i1 [[CONTROL:%.*]], label [[TAKEN:%.*]], label [[NOT_TAKEN:%.*]]
; CHECK:       taken:
; CHECK-NEXT:    [[LOAD:%.*]] = load i32*, i32** [[A:%.*]], align 8
; CHECK-NEXT:    [[CMP:%.*]] = icmp ne i32* [[LOAD]], null
; CHECK-NEXT:    [[CMP2:%.*]] = call i1 @tmp1(i1 [[CMP]])
; CHECK-NEXT:    br label [[EXIT:%.*]]
; CHECK:       exit:
; CHECK-NEXT:    ret i1 [[CMP2]]
; CHECK:       not_taken:
; CHECK-NEXT:    call void @llvm.assume(i1 true) [ "ignore"(i32* undef), "ignore"(i1 undef), "nonnull"(i1 [[CONTROL]]) ]
; CHECK-NEXT:    ret i1 [[CONTROL]]
;
entry:
  %load = load i32*, i32** %a
  %cmp = icmp ne i32* %load, null
  br i1 %control, label %taken, label %not_taken
taken:
  %cmp2 = call i1 @tmp1(i1 %cmp)
  br label %exit
exit:
  ret i1 %cmp2
not_taken:
  call void @llvm.assume(i1 %cmp) ["nonnull"(i32* %load), "nonnull"(i1 %cmp), "nonnull"(i1 %control)]
  ret i1 %control
}


define void @always_true_assumption() {
; CHECK-LABEL: @always_true_assumption(
; CHECK-NEXT:    ret void
;
  call void @llvm.assume(i1 true)
  ret void

}

; The alloca guarantees that the low bits of %a are zero because of alignment.
; The assume says the opposite. Make sure we don't crash.

define i64 @PR31809() {
; CHECK-LABEL: @PR31809(
; CHECK-NEXT:    [[A:%.*]] = alloca i32, align 4
; CHECK-NEXT:    [[T1:%.*]] = ptrtoint i32* [[A]] to i64
; CHECK-NEXT:    call void @llvm.assume(i1 false)
; CHECK-NEXT:    ret i64 [[T1]]
;
  %a = alloca i32
  %t1 = ptrtoint i32* %a to i64
  %cond = icmp eq i64 %t1, 3
  call void @llvm.assume(i1 %cond)
  ret i64 %t1
}

; Similar to above: there's no way to know which assumption is truthful,
; so just don't crash.

define i8 @conflicting_assumptions(i8 %x){
; CHECK-LABEL: @conflicting_assumptions(
; CHECK-NEXT:    call void @llvm.assume(i1 false)
; CHECK-NEXT:    [[COND2:%.*]] = icmp eq i8 [[X:%.*]], 4
; CHECK-NEXT:    call void @llvm.assume(i1 [[COND2]])
; CHECK-NEXT:    ret i8 5
;
  %add = add i8 %x, 1
  %cond1 = icmp eq i8 %x, 3
  call void @llvm.assume(i1 %cond1)
  %cond2 = icmp eq i8 %x, 4
  call void @llvm.assume(i1 %cond2)
  ret i8 %add
}

; Another case of conflicting assumptions. This would crash because we'd
; try to set more known bits than existed in the known bits struct.

define void @PR36270(i32 %b) {
; CHECK-LABEL: @PR36270(
; CHECK-NEXT:    tail call void @llvm.assume(i1 false)
; CHECK-NEXT:    unreachable
;
  %B7 = xor i32 -1, 2147483647
  %and1 = and i32 %b, 3
  %B12 = lshr i32 %B7, %and1
  %C1 = icmp ult i32 %and1, %B12
  tail call void @llvm.assume(i1 %C1)
  %cmp2 = icmp eq i32 0, %B12
  tail call void @llvm.assume(i1 %cmp2)
  unreachable
}

declare void @llvm.dbg.value(metadata, metadata, metadata)

!llvm.dbg.cu = !{!0}
!llvm.module.flags = !{!5, !6, !7, !8}

!0 = distinct !DICompileUnit(language: DW_LANG_C, file: !3, producer: "Me", isOptimized: true, runtimeVersion: 0, emissionKind: FullDebug, enums: null, retainedTypes: null, imports: null)
!1 = !DILocalVariable(name: "", arg: 1, scope: !2, file: null, line: 1, type: null)
!2 = distinct !DISubprogram(name: "debug", linkageName: "debug", scope: null, file: null, line: 0, type: null, isLocal: false, isDefinition: true, scopeLine: 1, flags: DIFlagPrototyped, isOptimized: true, unit: !0)
!3 = !DIFile(filename: "consecutive-fences.ll", directory: "")
!5 = !{i32 2, !"Dwarf Version", i32 4}
!6 = !{i32 2, !"Debug Info Version", i32 3}
!7 = !{i32 1, !"wchar_size", i32 4}
!8 = !{i32 7, !"PIC Level", i32 2}
!9 = !DILocation(line: 0, column: 0, scope: !2)


attributes #0 = { nounwind uwtable }
attributes #1 = { nounwind }

