; NOTE: Assertions have been autogenerated by utils/update_llc_test_checks.py
; RUN: llc -o - %s -mtriple aarch64-- -mattr +slow-misaligned-128store | FileCheck %s
; Checks for a bug where selection dag store merging would construct wrong
; indices when extracting values from vectors, resulting in an invalid
; lane duplication in this case.
; The only way I could trigger stores with mismatching types getting merged was
; via the aarch64 slow-misaligned-128store code splitting stores earlier.

; aarch64 feature slow-misaligned-128store splits the following store.
; store merging immediately merges it back together (but used to get the
; merging wrong), this is the only way I was able to reproduce the bug...

define void @func(<2 x double>* %sptr, <2 x double>* %dptr) {
; CHECK-LABEL: func:
; CHECK:       // %bb.0:
; CHECK-NEXT:    ldr q0, [x0]
; CHECK-NEXT:    str q0, [x1]
; CHECK-NEXT:    ret
  %load = load <2 x double>, <2 x double>* %sptr, align 8
  store <2 x double> %load, <2 x double>* %dptr, align 4
  ret void
}
