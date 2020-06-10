; NOTE: Assertions have been autogenerated by utils/update_test_checks.py
; RUN: opt -S -loop-vectorize -mtriple=x86_64-apple-darwin %s | FileCheck %s --check-prefixes=CHECK,SSE
; RUN: opt -S -loop-vectorize -mtriple=x86_64-apple-darwin -mattr=+avx %s | FileCheck %s --check-prefixes=CHECK,AVX

; Two mostly identical functions. The only difference is the presence of
; fast-math flags on the second. The loop is a pretty simple reduction:

; for (int i = 0; i < 32; ++i)
;   if (arr[i] != 42)
;     tot += arr[i];

define double @sumIfScalar(double* nocapture readonly %arr) {
; CHECK-LABEL: @sumIfScalar(
; CHECK-NEXT:  entry:
; CHECK-NEXT:    br label [[LOOP:%.*]]
; CHECK:       loop:
; CHECK-NEXT:    [[I:%.*]] = phi i32 [ 0, [[ENTRY:%.*]] ], [ [[I_NEXT:%.*]], [[NEXT_ITER:%.*]] ]
; CHECK-NEXT:    [[TOT:%.*]] = phi double [ 0.000000e+00, [[ENTRY]] ], [ [[TOT_NEXT:%.*]], [[NEXT_ITER]] ]
; CHECK-NEXT:    [[ADDR:%.*]] = getelementptr double, double* [[ARR:%.*]], i32 [[I]]
; CHECK-NEXT:    [[NEXTVAL:%.*]] = load double, double* [[ADDR]]
; CHECK-NEXT:    [[TST:%.*]] = fcmp une double [[NEXTVAL]], 4.200000e+01
; CHECK-NEXT:    br i1 [[TST]], label [[DO_ADD:%.*]], label [[NO_ADD:%.*]]
; CHECK:       do.add:
; CHECK-NEXT:    [[TOT_NEW:%.*]] = fadd double [[TOT]], [[NEXTVAL]]
; CHECK-NEXT:    br label [[NEXT_ITER]]
; CHECK:       no.add:
; CHECK-NEXT:    br label [[NEXT_ITER]]
; CHECK:       next.iter:
; CHECK-NEXT:    [[TOT_NEXT]] = phi double [ [[TOT]], [[NO_ADD]] ], [ [[TOT_NEW]], [[DO_ADD]] ]
; CHECK-NEXT:    [[I_NEXT]] = add i32 [[I]], 1
; CHECK-NEXT:    [[AGAIN:%.*]] = icmp ult i32 [[I_NEXT]], 32
; CHECK-NEXT:    br i1 [[AGAIN]], label [[LOOP]], label [[DONE:%.*]]
; CHECK:       done:
; CHECK-NEXT:    [[TOT_NEXT_LCSSA:%.*]] = phi double [ [[TOT_NEXT]], [[NEXT_ITER]] ]
; CHECK-NEXT:    ret double [[TOT_NEXT_LCSSA]]
;
entry:
  br label %loop

loop:
  %i = phi i32 [0, %entry], [%i.next, %next.iter]
  %tot = phi double [0.0, %entry], [%tot.next, %next.iter]

  %addr = getelementptr double, double* %arr, i32 %i
  %nextval = load double, double* %addr

  %tst = fcmp une double %nextval, 42.0
  br i1 %tst, label %do.add, label %no.add

do.add:
  %tot.new = fadd double %tot, %nextval
  br label %next.iter

no.add:
  br label %next.iter

next.iter:
  %tot.next = phi double [%tot, %no.add], [%tot.new, %do.add]
  %i.next = add i32 %i, 1
  %again = icmp ult i32 %i.next, 32
  br i1 %again, label %loop, label %done

done:
  ret double %tot.next
}

define double @sumIfVector(double* nocapture readonly %arr) {
; SSE-LABEL: @sumIfVector(
; SSE-NEXT:  entry:
; SSE-NEXT:    br label [[LOOP:%.*]]
; SSE:       loop:
; SSE-NEXT:    [[I:%.*]] = phi i32 [ 0, [[ENTRY:%.*]] ], [ [[I_NEXT:%.*]], [[NEXT_ITER:%.*]] ]
; SSE-NEXT:    [[TOT:%.*]] = phi double [ 0.000000e+00, [[ENTRY]] ], [ [[TOT_NEXT:%.*]], [[NEXT_ITER]] ]
; SSE-NEXT:    [[ADDR:%.*]] = getelementptr double, double* [[ARR:%.*]], i32 [[I]]
; SSE-NEXT:    [[NEXTVAL:%.*]] = load double, double* [[ADDR]]
; SSE-NEXT:    [[TST:%.*]] = fcmp fast une double [[NEXTVAL]], 4.200000e+01
; SSE-NEXT:    br i1 [[TST]], label [[DO_ADD:%.*]], label [[NO_ADD:%.*]]
; SSE:       do.add:
; SSE-NEXT:    [[TOT_NEW:%.*]] = fadd fast double [[TOT]], [[NEXTVAL]]
; SSE-NEXT:    br label [[NEXT_ITER]]
; SSE:       no.add:
; SSE-NEXT:    br label [[NEXT_ITER]]
; SSE:       next.iter:
; SSE-NEXT:    [[TOT_NEXT]] = phi double [ [[TOT]], [[NO_ADD]] ], [ [[TOT_NEW]], [[DO_ADD]] ]
; SSE-NEXT:    [[I_NEXT]] = add i32 [[I]], 1
; SSE-NEXT:    [[AGAIN:%.*]] = icmp ult i32 [[I_NEXT]], 32
; SSE-NEXT:    br i1 [[AGAIN]], label [[LOOP]], label [[DONE:%.*]]
; SSE:       done:
; SSE-NEXT:    [[TOT_NEXT_LCSSA:%.*]] = phi double [ [[TOT_NEXT]], [[NEXT_ITER]] ]
; SSE-NEXT:    ret double [[TOT_NEXT_LCSSA]]
;
; AVX-LABEL: @sumIfVector(
; AVX-NEXT:  entry:
; AVX-NEXT:    br i1 false, label [[SCALAR_PH:%.*]], label [[VECTOR_PH:%.*]]
; AVX:       vector.ph:
; AVX-NEXT:    br label [[VECTOR_BODY:%.*]]
; AVX:       vector.body:
; AVX-NEXT:    [[INDEX:%.*]] = phi i32 [ 0, [[VECTOR_PH]] ], [ [[INDEX_NEXT:%.*]], [[VECTOR_BODY]] ]
; AVX-NEXT:    [[VEC_PHI:%.*]] = phi <4 x double> [ zeroinitializer, [[VECTOR_PH]] ], [ [[PREDPHI:%.*]], [[VECTOR_BODY]] ]
; AVX-NEXT:    [[TMP0:%.*]] = add i32 [[INDEX]], 0
; AVX-NEXT:    [[TMP1:%.*]] = getelementptr double, double* [[ARR:%.*]], i32 [[TMP0]]
; AVX-NEXT:    [[TMP2:%.*]] = getelementptr double, double* [[TMP1]], i32 0
; AVX-NEXT:    [[TMP3:%.*]] = bitcast double* [[TMP2]] to <4 x double>*
; AVX-NEXT:    [[WIDE_LOAD:%.*]] = load <4 x double>, <4 x double>* [[TMP3]], align 8
; AVX-NEXT:    [[TMP4:%.*]] = fcmp fast une <4 x double> [[WIDE_LOAD]], <double 4.200000e+01, double 4.200000e+01, double 4.200000e+01, double 4.200000e+01>
; AVX-NEXT:    [[TMP5:%.*]] = fadd fast <4 x double> [[VEC_PHI]], [[WIDE_LOAD]]
; AVX-NEXT:    [[TMP6:%.*]] = xor <4 x i1> [[TMP4]], <i1 true, i1 true, i1 true, i1 true>
; AVX-NEXT:    [[PREDPHI]] = select <4 x i1> [[TMP4]], <4 x double> [[TMP5]], <4 x double> [[VEC_PHI]]
; AVX-NEXT:    [[INDEX_NEXT]] = add i32 [[INDEX]], 4
; AVX-NEXT:    [[TMP7:%.*]] = icmp eq i32 [[INDEX_NEXT]], 32
; AVX-NEXT:    br i1 [[TMP7]], label [[MIDDLE_BLOCK:%.*]], label [[VECTOR_BODY]], !llvm.loop !0
; AVX:       middle.block:
; AVX-NEXT:    [[RDX_SHUF:%.*]] = shufflevector <4 x double> [[PREDPHI]], <4 x double> undef, <4 x i32> <i32 2, i32 3, i32 undef, i32 undef>
; AVX-NEXT:    [[BIN_RDX:%.*]] = fadd fast <4 x double> [[PREDPHI]], [[RDX_SHUF]]
; AVX-NEXT:    [[RDX_SHUF1:%.*]] = shufflevector <4 x double> [[BIN_RDX]], <4 x double> undef, <4 x i32> <i32 1, i32 undef, i32 undef, i32 undef>
; AVX-NEXT:    [[BIN_RDX2:%.*]] = fadd fast <4 x double> [[BIN_RDX]], [[RDX_SHUF1]]
; AVX-NEXT:    [[TMP8:%.*]] = extractelement <4 x double> [[BIN_RDX2]], i32 0
; AVX-NEXT:    [[CMP_N:%.*]] = icmp eq i32 32, 32
; AVX-NEXT:    br i1 [[CMP_N]], label [[DONE:%.*]], label [[SCALAR_PH]]
; AVX:       scalar.ph:
; AVX-NEXT:    [[BC_RESUME_VAL:%.*]] = phi i32 [ 32, [[MIDDLE_BLOCK]] ], [ 0, [[ENTRY:%.*]] ]
; AVX-NEXT:    [[BC_MERGE_RDX:%.*]] = phi double [ 0.000000e+00, [[ENTRY]] ], [ [[TMP8]], [[MIDDLE_BLOCK]] ]
; AVX-NEXT:    br label [[LOOP:%.*]]
; AVX:       loop:
; AVX-NEXT:    [[I:%.*]] = phi i32 [ [[BC_RESUME_VAL]], [[SCALAR_PH]] ], [ [[I_NEXT:%.*]], [[NEXT_ITER:%.*]] ]
; AVX-NEXT:    [[TOT:%.*]] = phi double [ [[BC_MERGE_RDX]], [[SCALAR_PH]] ], [ [[TOT_NEXT:%.*]], [[NEXT_ITER]] ]
; AVX-NEXT:    [[ADDR:%.*]] = getelementptr double, double* [[ARR]], i32 [[I]]
; AVX-NEXT:    [[NEXTVAL:%.*]] = load double, double* [[ADDR]]
; AVX-NEXT:    [[TST:%.*]] = fcmp fast une double [[NEXTVAL]], 4.200000e+01
; AVX-NEXT:    br i1 [[TST]], label [[DO_ADD:%.*]], label [[NO_ADD:%.*]]
; AVX:       do.add:
; AVX-NEXT:    [[TOT_NEW:%.*]] = fadd fast double [[TOT]], [[NEXTVAL]]
; AVX-NEXT:    br label [[NEXT_ITER]]
; AVX:       no.add:
; AVX-NEXT:    br label [[NEXT_ITER]]
; AVX:       next.iter:
; AVX-NEXT:    [[TOT_NEXT]] = phi double [ [[TOT]], [[NO_ADD]] ], [ [[TOT_NEW]], [[DO_ADD]] ]
; AVX-NEXT:    [[I_NEXT]] = add i32 [[I]], 1
; AVX-NEXT:    [[AGAIN:%.*]] = icmp ult i32 [[I_NEXT]], 32
; AVX-NEXT:    br i1 [[AGAIN]], label [[LOOP]], label [[DONE]], !llvm.loop !2
; AVX:       done:
; AVX-NEXT:    [[TOT_NEXT_LCSSA:%.*]] = phi double [ [[TOT_NEXT]], [[NEXT_ITER]] ], [ [[TMP8]], [[MIDDLE_BLOCK]] ]
; AVX-NEXT:    ret double [[TOT_NEXT_LCSSA]]
;
entry:
  br label %loop

loop:
  %i = phi i32 [0, %entry], [%i.next, %next.iter]
  %tot = phi double [0.0, %entry], [%tot.next, %next.iter]

  %addr = getelementptr double, double* %arr, i32 %i
  %nextval = load double, double* %addr

  %tst = fcmp fast une double %nextval, 42.0
  br i1 %tst, label %do.add, label %no.add

do.add:
  %tot.new = fadd fast double %tot, %nextval
  br label %next.iter

no.add:
  br label %next.iter

next.iter:
  %tot.next = phi double [%tot, %no.add], [%tot.new, %do.add]
  %i.next = add i32 %i, 1
  %again = icmp ult i32 %i.next, 32
  br i1 %again, label %loop, label %done

done:
  ret double %tot.next
}
