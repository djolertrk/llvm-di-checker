// NOTE: Assertions have been autogenerated by utils/update_cc_test_checks.py
// RUN: %clang_cc1 -triple thumbv8.1m.main-none-none-eabi -target-feature +mve.fp -mfloat-abi hard -fallow-half-arguments-and-returns -O0 -disable-O0-optnone -S -emit-llvm -o - %s | opt -S -mem2reg | FileCheck %s
// RUN: %clang_cc1 -triple thumbv8.1m.main-none-none-eabi -target-feature +mve.fp -mfloat-abi hard -fallow-half-arguments-and-returns -O0 -disable-O0-optnone -DPOLYMORPHIC -S -emit-llvm -o - %s | opt -S -mem2reg -sroa -early-cse | FileCheck %s

#include <arm_mve.h>

// CHECK-LABEL: @test_vrev16q_s8(
// CHECK-NEXT:  entry:
// CHECK-NEXT:    [[TMP0:%.*]] = shufflevector <16 x i8> [[A:%.*]], <16 x i8> undef, <16 x i32> <i32 1, i32 0, i32 3, i32 2, i32 5, i32 4, i32 7, i32 6, i32 9, i32 8, i32 11, i32 10, i32 13, i32 12, i32 15, i32 14>
// CHECK-NEXT:    ret <16 x i8> [[TMP0]]
//
int8x16_t test_vrev16q_s8(int8x16_t a)
{
#ifdef POLYMORPHIC
    return vrev16q(a);
#else /* POLYMORPHIC */
    return vrev16q_s8(a);
#endif /* POLYMORPHIC */
}

// CHECK-LABEL: @test_vrev16q_u8(
// CHECK-NEXT:  entry:
// CHECK-NEXT:    [[TMP0:%.*]] = shufflevector <16 x i8> [[A:%.*]], <16 x i8> undef, <16 x i32> <i32 1, i32 0, i32 3, i32 2, i32 5, i32 4, i32 7, i32 6, i32 9, i32 8, i32 11, i32 10, i32 13, i32 12, i32 15, i32 14>
// CHECK-NEXT:    ret <16 x i8> [[TMP0]]
//
uint8x16_t test_vrev16q_u8(uint8x16_t a)
{
#ifdef POLYMORPHIC
    return vrev16q(a);
#else /* POLYMORPHIC */
    return vrev16q_u8(a);
#endif /* POLYMORPHIC */
}

// CHECK-LABEL: @test_vrev32q_s8(
// CHECK-NEXT:  entry:
// CHECK-NEXT:    [[TMP0:%.*]] = shufflevector <16 x i8> [[A:%.*]], <16 x i8> undef, <16 x i32> <i32 3, i32 2, i32 1, i32 0, i32 7, i32 6, i32 5, i32 4, i32 11, i32 10, i32 9, i32 8, i32 15, i32 14, i32 13, i32 12>
// CHECK-NEXT:    ret <16 x i8> [[TMP0]]
//
int8x16_t test_vrev32q_s8(int8x16_t a)
{
#ifdef POLYMORPHIC
    return vrev32q(a);
#else /* POLYMORPHIC */
    return vrev32q_s8(a);
#endif /* POLYMORPHIC */
}

// CHECK-LABEL: @test_vrev32q_u8(
// CHECK-NEXT:  entry:
// CHECK-NEXT:    [[TMP0:%.*]] = shufflevector <16 x i8> [[A:%.*]], <16 x i8> undef, <16 x i32> <i32 3, i32 2, i32 1, i32 0, i32 7, i32 6, i32 5, i32 4, i32 11, i32 10, i32 9, i32 8, i32 15, i32 14, i32 13, i32 12>
// CHECK-NEXT:    ret <16 x i8> [[TMP0]]
//
uint8x16_t test_vrev32q_u8(uint8x16_t a)
{
#ifdef POLYMORPHIC
    return vrev32q(a);
#else /* POLYMORPHIC */
    return vrev32q_u8(a);
#endif /* POLYMORPHIC */
}

// CHECK-LABEL: @test_vrev32q_s16(
// CHECK-NEXT:  entry:
// CHECK-NEXT:    [[TMP0:%.*]] = shufflevector <8 x i16> [[A:%.*]], <8 x i16> undef, <8 x i32> <i32 1, i32 0, i32 3, i32 2, i32 5, i32 4, i32 7, i32 6>
// CHECK-NEXT:    ret <8 x i16> [[TMP0]]
//
int16x8_t test_vrev32q_s16(int16x8_t a)
{
#ifdef POLYMORPHIC
    return vrev32q(a);
#else /* POLYMORPHIC */
    return vrev32q_s16(a);
#endif /* POLYMORPHIC */
}

// CHECK-LABEL: @test_vrev32q_u16(
// CHECK-NEXT:  entry:
// CHECK-NEXT:    [[TMP0:%.*]] = shufflevector <8 x i16> [[A:%.*]], <8 x i16> undef, <8 x i32> <i32 1, i32 0, i32 3, i32 2, i32 5, i32 4, i32 7, i32 6>
// CHECK-NEXT:    ret <8 x i16> [[TMP0]]
//
uint16x8_t test_vrev32q_u16(uint16x8_t a)
{
#ifdef POLYMORPHIC
    return vrev32q(a);
#else /* POLYMORPHIC */
    return vrev32q_u16(a);
#endif /* POLYMORPHIC */
}

// CHECK-LABEL: @test_vrev32q_f16(
// CHECK-NEXT:  entry:
// CHECK-NEXT:    [[TMP0:%.*]] = shufflevector <8 x half> [[A:%.*]], <8 x half> undef, <8 x i32> <i32 1, i32 0, i32 3, i32 2, i32 5, i32 4, i32 7, i32 6>
// CHECK-NEXT:    ret <8 x half> [[TMP0]]
//
float16x8_t test_vrev32q_f16(float16x8_t a)
{
#ifdef POLYMORPHIC
    return vrev32q(a);
#else /* POLYMORPHIC */
    return vrev32q_f16(a);
#endif /* POLYMORPHIC */
}

// CHECK-LABEL: @test_vrev64q_s8(
// CHECK-NEXT:  entry:
// CHECK-NEXT:    [[TMP0:%.*]] = shufflevector <16 x i8> [[A:%.*]], <16 x i8> undef, <16 x i32> <i32 7, i32 6, i32 5, i32 4, i32 3, i32 2, i32 1, i32 0, i32 15, i32 14, i32 13, i32 12, i32 11, i32 10, i32 9, i32 8>
// CHECK-NEXT:    ret <16 x i8> [[TMP0]]
//
int8x16_t test_vrev64q_s8(int8x16_t a)
{
#ifdef POLYMORPHIC
    return vrev64q(a);
#else /* POLYMORPHIC */
    return vrev64q_s8(a);
#endif /* POLYMORPHIC */
}

// CHECK-LABEL: @test_vrev64q_u8(
// CHECK-NEXT:  entry:
// CHECK-NEXT:    [[TMP0:%.*]] = shufflevector <16 x i8> [[A:%.*]], <16 x i8> undef, <16 x i32> <i32 7, i32 6, i32 5, i32 4, i32 3, i32 2, i32 1, i32 0, i32 15, i32 14, i32 13, i32 12, i32 11, i32 10, i32 9, i32 8>
// CHECK-NEXT:    ret <16 x i8> [[TMP0]]
//
uint8x16_t test_vrev64q_u8(uint8x16_t a)
{
#ifdef POLYMORPHIC
    return vrev64q(a);
#else /* POLYMORPHIC */
    return vrev64q_u8(a);
#endif /* POLYMORPHIC */
}

// CHECK-LABEL: @test_vrev64q_s16(
// CHECK-NEXT:  entry:
// CHECK-NEXT:    [[TMP0:%.*]] = shufflevector <8 x i16> [[A:%.*]], <8 x i16> undef, <8 x i32> <i32 3, i32 2, i32 1, i32 0, i32 7, i32 6, i32 5, i32 4>
// CHECK-NEXT:    ret <8 x i16> [[TMP0]]
//
int16x8_t test_vrev64q_s16(int16x8_t a)
{
#ifdef POLYMORPHIC
    return vrev64q(a);
#else /* POLYMORPHIC */
    return vrev64q_s16(a);
#endif /* POLYMORPHIC */
}

// CHECK-LABEL: @test_vrev64q_u16(
// CHECK-NEXT:  entry:
// CHECK-NEXT:    [[TMP0:%.*]] = shufflevector <8 x i16> [[A:%.*]], <8 x i16> undef, <8 x i32> <i32 3, i32 2, i32 1, i32 0, i32 7, i32 6, i32 5, i32 4>
// CHECK-NEXT:    ret <8 x i16> [[TMP0]]
//
uint16x8_t test_vrev64q_u16(uint16x8_t a)
{
#ifdef POLYMORPHIC
    return vrev64q(a);
#else /* POLYMORPHIC */
    return vrev64q_u16(a);
#endif /* POLYMORPHIC */
}

// CHECK-LABEL: @test_vrev64q_f16(
// CHECK-NEXT:  entry:
// CHECK-NEXT:    [[TMP0:%.*]] = shufflevector <8 x half> [[A:%.*]], <8 x half> undef, <8 x i32> <i32 3, i32 2, i32 1, i32 0, i32 7, i32 6, i32 5, i32 4>
// CHECK-NEXT:    ret <8 x half> [[TMP0]]
//
float16x8_t test_vrev64q_f16(float16x8_t a)
{
#ifdef POLYMORPHIC
    return vrev64q(a);
#else /* POLYMORPHIC */
    return vrev64q_f16(a);
#endif /* POLYMORPHIC */
}

// CHECK-LABEL: @test_vrev64q_f32(
// CHECK-NEXT:  entry:
// CHECK-NEXT:    [[TMP0:%.*]] = shufflevector <4 x float> [[A:%.*]], <4 x float> undef, <4 x i32> <i32 1, i32 0, i32 3, i32 2>
// CHECK-NEXT:    ret <4 x float> [[TMP0]]
//
float32x4_t test_vrev64q_f32(float32x4_t a)
{
#ifdef POLYMORPHIC
    return vrev64q(a);
#else /* POLYMORPHIC */
    return vrev64q_f32(a);
#endif /* POLYMORPHIC */
}

// CHECK-LABEL: @test_vrev64q_s32(
// CHECK-NEXT:  entry:
// CHECK-NEXT:    [[TMP0:%.*]] = shufflevector <4 x i32> [[A:%.*]], <4 x i32> undef, <4 x i32> <i32 1, i32 0, i32 3, i32 2>
// CHECK-NEXT:    ret <4 x i32> [[TMP0]]
//
int32x4_t test_vrev64q_s32(int32x4_t a)
{
#ifdef POLYMORPHIC
    return vrev64q(a);
#else /* POLYMORPHIC */
    return vrev64q_s32(a);
#endif /* POLYMORPHIC */
}

// CHECK-LABEL: @test_vrev64q_u32(
// CHECK-NEXT:  entry:
// CHECK-NEXT:    [[TMP0:%.*]] = shufflevector <4 x i32> [[A:%.*]], <4 x i32> undef, <4 x i32> <i32 1, i32 0, i32 3, i32 2>
// CHECK-NEXT:    ret <4 x i32> [[TMP0]]
//
uint32x4_t test_vrev64q_u32(uint32x4_t a)
{
#ifdef POLYMORPHIC
    return vrev64q(a);
#else /* POLYMORPHIC */
    return vrev64q_u32(a);
#endif /* POLYMORPHIC */
}

// CHECK-LABEL: @test_vrev16q_m_s8(
// CHECK-NEXT:  entry:
// CHECK-NEXT:    [[TMP0:%.*]] = zext i16 [[P:%.*]] to i32
// CHECK-NEXT:    [[TMP1:%.*]] = call <16 x i1> @llvm.arm.mve.pred.i2v.v16i1(i32 [[TMP0]])
// CHECK-NEXT:    [[TMP2:%.*]] = call <16 x i8> @llvm.arm.mve.vrev.predicated.v16i8.v16i1(<16 x i8> [[A:%.*]], i32 16, <16 x i1> [[TMP1]], <16 x i8> [[INACTIVE:%.*]])
// CHECK-NEXT:    ret <16 x i8> [[TMP2]]
//
int8x16_t test_vrev16q_m_s8(int8x16_t inactive, int8x16_t a, mve_pred16_t p)
{
#ifdef POLYMORPHIC
    return vrev16q_m(inactive, a, p);
#else /* POLYMORPHIC */
    return vrev16q_m_s8(inactive, a, p);
#endif /* POLYMORPHIC */
}

// CHECK-LABEL: @test_vrev16q_m_u8(
// CHECK-NEXT:  entry:
// CHECK-NEXT:    [[TMP0:%.*]] = zext i16 [[P:%.*]] to i32
// CHECK-NEXT:    [[TMP1:%.*]] = call <16 x i1> @llvm.arm.mve.pred.i2v.v16i1(i32 [[TMP0]])
// CHECK-NEXT:    [[TMP2:%.*]] = call <16 x i8> @llvm.arm.mve.vrev.predicated.v16i8.v16i1(<16 x i8> [[A:%.*]], i32 16, <16 x i1> [[TMP1]], <16 x i8> [[INACTIVE:%.*]])
// CHECK-NEXT:    ret <16 x i8> [[TMP2]]
//
uint8x16_t test_vrev16q_m_u8(uint8x16_t inactive, uint8x16_t a, mve_pred16_t p)
{
#ifdef POLYMORPHIC
    return vrev16q_m(inactive, a, p);
#else /* POLYMORPHIC */
    return vrev16q_m_u8(inactive, a, p);
#endif /* POLYMORPHIC */
}

// CHECK-LABEL: @test_vrev32q_m_f16(
// CHECK-NEXT:  entry:
// CHECK-NEXT:    [[TMP0:%.*]] = zext i16 [[P:%.*]] to i32
// CHECK-NEXT:    [[TMP1:%.*]] = call <8 x i1> @llvm.arm.mve.pred.i2v.v8i1(i32 [[TMP0]])
// CHECK-NEXT:    [[TMP2:%.*]] = call <8 x half> @llvm.arm.mve.vrev.predicated.v8f16.v8i1(<8 x half> [[A:%.*]], i32 32, <8 x i1> [[TMP1]], <8 x half> [[INACTIVE:%.*]])
// CHECK-NEXT:    ret <8 x half> [[TMP2]]
//
float16x8_t test_vrev32q_m_f16(float16x8_t inactive, float16x8_t a, mve_pred16_t p)
{
#ifdef POLYMORPHIC
    return vrev32q_m(inactive, a, p);
#else /* POLYMORPHIC */
    return vrev32q_m_f16(inactive, a, p);
#endif /* POLYMORPHIC */
}

// CHECK-LABEL: @test_vrev32q_m_s8(
// CHECK-NEXT:  entry:
// CHECK-NEXT:    [[TMP0:%.*]] = zext i16 [[P:%.*]] to i32
// CHECK-NEXT:    [[TMP1:%.*]] = call <16 x i1> @llvm.arm.mve.pred.i2v.v16i1(i32 [[TMP0]])
// CHECK-NEXT:    [[TMP2:%.*]] = call <16 x i8> @llvm.arm.mve.vrev.predicated.v16i8.v16i1(<16 x i8> [[A:%.*]], i32 32, <16 x i1> [[TMP1]], <16 x i8> [[INACTIVE:%.*]])
// CHECK-NEXT:    ret <16 x i8> [[TMP2]]
//
int8x16_t test_vrev32q_m_s8(int8x16_t inactive, int8x16_t a, mve_pred16_t p)
{
#ifdef POLYMORPHIC
    return vrev32q_m(inactive, a, p);
#else /* POLYMORPHIC */
    return vrev32q_m_s8(inactive, a, p);
#endif /* POLYMORPHIC */
}

// CHECK-LABEL: @test_vrev32q_m_s16(
// CHECK-NEXT:  entry:
// CHECK-NEXT:    [[TMP0:%.*]] = zext i16 [[P:%.*]] to i32
// CHECK-NEXT:    [[TMP1:%.*]] = call <8 x i1> @llvm.arm.mve.pred.i2v.v8i1(i32 [[TMP0]])
// CHECK-NEXT:    [[TMP2:%.*]] = call <8 x i16> @llvm.arm.mve.vrev.predicated.v8i16.v8i1(<8 x i16> [[A:%.*]], i32 32, <8 x i1> [[TMP1]], <8 x i16> [[INACTIVE:%.*]])
// CHECK-NEXT:    ret <8 x i16> [[TMP2]]
//
int16x8_t test_vrev32q_m_s16(int16x8_t inactive, int16x8_t a, mve_pred16_t p)
{
#ifdef POLYMORPHIC
    return vrev32q_m(inactive, a, p);
#else /* POLYMORPHIC */
    return vrev32q_m_s16(inactive, a, p);
#endif /* POLYMORPHIC */
}

// CHECK-LABEL: @test_vrev32q_m_u8(
// CHECK-NEXT:  entry:
// CHECK-NEXT:    [[TMP0:%.*]] = zext i16 [[P:%.*]] to i32
// CHECK-NEXT:    [[TMP1:%.*]] = call <16 x i1> @llvm.arm.mve.pred.i2v.v16i1(i32 [[TMP0]])
// CHECK-NEXT:    [[TMP2:%.*]] = call <16 x i8> @llvm.arm.mve.vrev.predicated.v16i8.v16i1(<16 x i8> [[A:%.*]], i32 32, <16 x i1> [[TMP1]], <16 x i8> [[INACTIVE:%.*]])
// CHECK-NEXT:    ret <16 x i8> [[TMP2]]
//
uint8x16_t test_vrev32q_m_u8(uint8x16_t inactive, uint8x16_t a, mve_pred16_t p)
{
#ifdef POLYMORPHIC
    return vrev32q_m(inactive, a, p);
#else /* POLYMORPHIC */
    return vrev32q_m_u8(inactive, a, p);
#endif /* POLYMORPHIC */
}

// CHECK-LABEL: @test_vrev32q_m_u16(
// CHECK-NEXT:  entry:
// CHECK-NEXT:    [[TMP0:%.*]] = zext i16 [[P:%.*]] to i32
// CHECK-NEXT:    [[TMP1:%.*]] = call <8 x i1> @llvm.arm.mve.pred.i2v.v8i1(i32 [[TMP0]])
// CHECK-NEXT:    [[TMP2:%.*]] = call <8 x i16> @llvm.arm.mve.vrev.predicated.v8i16.v8i1(<8 x i16> [[A:%.*]], i32 32, <8 x i1> [[TMP1]], <8 x i16> [[INACTIVE:%.*]])
// CHECK-NEXT:    ret <8 x i16> [[TMP2]]
//
uint16x8_t test_vrev32q_m_u16(uint16x8_t inactive, uint16x8_t a, mve_pred16_t p)
{
#ifdef POLYMORPHIC
    return vrev32q_m(inactive, a, p);
#else /* POLYMORPHIC */
    return vrev32q_m_u16(inactive, a, p);
#endif /* POLYMORPHIC */
}

// CHECK-LABEL: @test_vrev64q_m_f16(
// CHECK-NEXT:  entry:
// CHECK-NEXT:    [[TMP0:%.*]] = zext i16 [[P:%.*]] to i32
// CHECK-NEXT:    [[TMP1:%.*]] = call <8 x i1> @llvm.arm.mve.pred.i2v.v8i1(i32 [[TMP0]])
// CHECK-NEXT:    [[TMP2:%.*]] = call <8 x half> @llvm.arm.mve.vrev.predicated.v8f16.v8i1(<8 x half> [[A:%.*]], i32 64, <8 x i1> [[TMP1]], <8 x half> [[INACTIVE:%.*]])
// CHECK-NEXT:    ret <8 x half> [[TMP2]]
//
float16x8_t test_vrev64q_m_f16(float16x8_t inactive, float16x8_t a, mve_pred16_t p)
{
#ifdef POLYMORPHIC
    return vrev64q_m(inactive, a, p);
#else /* POLYMORPHIC */
    return vrev64q_m_f16(inactive, a, p);
#endif /* POLYMORPHIC */
}

// CHECK-LABEL: @test_vrev64q_m_f32(
// CHECK-NEXT:  entry:
// CHECK-NEXT:    [[TMP0:%.*]] = zext i16 [[P:%.*]] to i32
// CHECK-NEXT:    [[TMP1:%.*]] = call <4 x i1> @llvm.arm.mve.pred.i2v.v4i1(i32 [[TMP0]])
// CHECK-NEXT:    [[TMP2:%.*]] = call <4 x float> @llvm.arm.mve.vrev.predicated.v4f32.v4i1(<4 x float> [[A:%.*]], i32 64, <4 x i1> [[TMP1]], <4 x float> [[INACTIVE:%.*]])
// CHECK-NEXT:    ret <4 x float> [[TMP2]]
//
float32x4_t test_vrev64q_m_f32(float32x4_t inactive, float32x4_t a, mve_pred16_t p)
{
#ifdef POLYMORPHIC
    return vrev64q_m(inactive, a, p);
#else /* POLYMORPHIC */
    return vrev64q_m_f32(inactive, a, p);
#endif /* POLYMORPHIC */
}

// CHECK-LABEL: @test_vrev64q_m_s8(
// CHECK-NEXT:  entry:
// CHECK-NEXT:    [[TMP0:%.*]] = zext i16 [[P:%.*]] to i32
// CHECK-NEXT:    [[TMP1:%.*]] = call <16 x i1> @llvm.arm.mve.pred.i2v.v16i1(i32 [[TMP0]])
// CHECK-NEXT:    [[TMP2:%.*]] = call <16 x i8> @llvm.arm.mve.vrev.predicated.v16i8.v16i1(<16 x i8> [[A:%.*]], i32 64, <16 x i1> [[TMP1]], <16 x i8> [[INACTIVE:%.*]])
// CHECK-NEXT:    ret <16 x i8> [[TMP2]]
//
int8x16_t test_vrev64q_m_s8(int8x16_t inactive, int8x16_t a, mve_pred16_t p)
{
#ifdef POLYMORPHIC
    return vrev64q_m(inactive, a, p);
#else /* POLYMORPHIC */
    return vrev64q_m_s8(inactive, a, p);
#endif /* POLYMORPHIC */
}

// CHECK-LABEL: @test_vrev64q_m_s16(
// CHECK-NEXT:  entry:
// CHECK-NEXT:    [[TMP0:%.*]] = zext i16 [[P:%.*]] to i32
// CHECK-NEXT:    [[TMP1:%.*]] = call <8 x i1> @llvm.arm.mve.pred.i2v.v8i1(i32 [[TMP0]])
// CHECK-NEXT:    [[TMP2:%.*]] = call <8 x i16> @llvm.arm.mve.vrev.predicated.v8i16.v8i1(<8 x i16> [[A:%.*]], i32 64, <8 x i1> [[TMP1]], <8 x i16> [[INACTIVE:%.*]])
// CHECK-NEXT:    ret <8 x i16> [[TMP2]]
//
int16x8_t test_vrev64q_m_s16(int16x8_t inactive, int16x8_t a, mve_pred16_t p)
{
#ifdef POLYMORPHIC
    return vrev64q_m(inactive, a, p);
#else /* POLYMORPHIC */
    return vrev64q_m_s16(inactive, a, p);
#endif /* POLYMORPHIC */
}

// CHECK-LABEL: @test_vrev64q_m_s32(
// CHECK-NEXT:  entry:
// CHECK-NEXT:    [[TMP0:%.*]] = zext i16 [[P:%.*]] to i32
// CHECK-NEXT:    [[TMP1:%.*]] = call <4 x i1> @llvm.arm.mve.pred.i2v.v4i1(i32 [[TMP0]])
// CHECK-NEXT:    [[TMP2:%.*]] = call <4 x i32> @llvm.arm.mve.vrev.predicated.v4i32.v4i1(<4 x i32> [[A:%.*]], i32 64, <4 x i1> [[TMP1]], <4 x i32> [[INACTIVE:%.*]])
// CHECK-NEXT:    ret <4 x i32> [[TMP2]]
//
int32x4_t test_vrev64q_m_s32(int32x4_t inactive, int32x4_t a, mve_pred16_t p)
{
#ifdef POLYMORPHIC
    return vrev64q_m(inactive, a, p);
#else /* POLYMORPHIC */
    return vrev64q_m_s32(inactive, a, p);
#endif /* POLYMORPHIC */
}

// CHECK-LABEL: @test_vrev64q_m_u8(
// CHECK-NEXT:  entry:
// CHECK-NEXT:    [[TMP0:%.*]] = zext i16 [[P:%.*]] to i32
// CHECK-NEXT:    [[TMP1:%.*]] = call <16 x i1> @llvm.arm.mve.pred.i2v.v16i1(i32 [[TMP0]])
// CHECK-NEXT:    [[TMP2:%.*]] = call <16 x i8> @llvm.arm.mve.vrev.predicated.v16i8.v16i1(<16 x i8> [[A:%.*]], i32 64, <16 x i1> [[TMP1]], <16 x i8> [[INACTIVE:%.*]])
// CHECK-NEXT:    ret <16 x i8> [[TMP2]]
//
uint8x16_t test_vrev64q_m_u8(uint8x16_t inactive, uint8x16_t a, mve_pred16_t p)
{
#ifdef POLYMORPHIC
    return vrev64q_m(inactive, a, p);
#else /* POLYMORPHIC */
    return vrev64q_m_u8(inactive, a, p);
#endif /* POLYMORPHIC */
}

// CHECK-LABEL: @test_vrev64q_m_u16(
// CHECK-NEXT:  entry:
// CHECK-NEXT:    [[TMP0:%.*]] = zext i16 [[P:%.*]] to i32
// CHECK-NEXT:    [[TMP1:%.*]] = call <8 x i1> @llvm.arm.mve.pred.i2v.v8i1(i32 [[TMP0]])
// CHECK-NEXT:    [[TMP2:%.*]] = call <8 x i16> @llvm.arm.mve.vrev.predicated.v8i16.v8i1(<8 x i16> [[A:%.*]], i32 64, <8 x i1> [[TMP1]], <8 x i16> [[INACTIVE:%.*]])
// CHECK-NEXT:    ret <8 x i16> [[TMP2]]
//
uint16x8_t test_vrev64q_m_u16(uint16x8_t inactive, uint16x8_t a, mve_pred16_t p)
{
#ifdef POLYMORPHIC
    return vrev64q_m(inactive, a, p);
#else /* POLYMORPHIC */
    return vrev64q_m_u16(inactive, a, p);
#endif /* POLYMORPHIC */
}

// CHECK-LABEL: @test_vrev64q_m_u32(
// CHECK-NEXT:  entry:
// CHECK-NEXT:    [[TMP0:%.*]] = zext i16 [[P:%.*]] to i32
// CHECK-NEXT:    [[TMP1:%.*]] = call <4 x i1> @llvm.arm.mve.pred.i2v.v4i1(i32 [[TMP0]])
// CHECK-NEXT:    [[TMP2:%.*]] = call <4 x i32> @llvm.arm.mve.vrev.predicated.v4i32.v4i1(<4 x i32> [[A:%.*]], i32 64, <4 x i1> [[TMP1]], <4 x i32> [[INACTIVE:%.*]])
// CHECK-NEXT:    ret <4 x i32> [[TMP2]]
//
uint32x4_t test_vrev64q_m_u32(uint32x4_t inactive, uint32x4_t a, mve_pred16_t p)
{
#ifdef POLYMORPHIC
    return vrev64q_m(inactive, a, p);
#else /* POLYMORPHIC */
    return vrev64q_m_u32(inactive, a, p);
#endif /* POLYMORPHIC */
}

// CHECK-LABEL: @test_vrev16q_x_s8(
// CHECK-NEXT:  entry:
// CHECK-NEXT:    [[TMP0:%.*]] = zext i16 [[P:%.*]] to i32
// CHECK-NEXT:    [[TMP1:%.*]] = call <16 x i1> @llvm.arm.mve.pred.i2v.v16i1(i32 [[TMP0]])
// CHECK-NEXT:    [[TMP2:%.*]] = call <16 x i8> @llvm.arm.mve.vrev.predicated.v16i8.v16i1(<16 x i8> [[A:%.*]], i32 16, <16 x i1> [[TMP1]], <16 x i8> undef)
// CHECK-NEXT:    ret <16 x i8> [[TMP2]]
//
int8x16_t test_vrev16q_x_s8(int8x16_t a, mve_pred16_t p)
{
#ifdef POLYMORPHIC
    return vrev16q_x(a, p);
#else /* POLYMORPHIC */
    return vrev16q_x_s8(a, p);
#endif /* POLYMORPHIC */
}

// CHECK-LABEL: @test_vrev16q_x_u8(
// CHECK-NEXT:  entry:
// CHECK-NEXT:    [[TMP0:%.*]] = zext i16 [[P:%.*]] to i32
// CHECK-NEXT:    [[TMP1:%.*]] = call <16 x i1> @llvm.arm.mve.pred.i2v.v16i1(i32 [[TMP0]])
// CHECK-NEXT:    [[TMP2:%.*]] = call <16 x i8> @llvm.arm.mve.vrev.predicated.v16i8.v16i1(<16 x i8> [[A:%.*]], i32 16, <16 x i1> [[TMP1]], <16 x i8> undef)
// CHECK-NEXT:    ret <16 x i8> [[TMP2]]
//
uint8x16_t test_vrev16q_x_u8(uint8x16_t a, mve_pred16_t p)
{
#ifdef POLYMORPHIC
    return vrev16q_x(a, p);
#else /* POLYMORPHIC */
    return vrev16q_x_u8(a, p);
#endif /* POLYMORPHIC */
}

// CHECK-LABEL: @test_vrev32q_x_f16(
// CHECK-NEXT:  entry:
// CHECK-NEXT:    [[TMP0:%.*]] = zext i16 [[P:%.*]] to i32
// CHECK-NEXT:    [[TMP1:%.*]] = call <8 x i1> @llvm.arm.mve.pred.i2v.v8i1(i32 [[TMP0]])
// CHECK-NEXT:    [[TMP2:%.*]] = call <8 x half> @llvm.arm.mve.vrev.predicated.v8f16.v8i1(<8 x half> [[A:%.*]], i32 32, <8 x i1> [[TMP1]], <8 x half> undef)
// CHECK-NEXT:    ret <8 x half> [[TMP2]]
//
float16x8_t test_vrev32q_x_f16(float16x8_t a, mve_pred16_t p)
{
#ifdef POLYMORPHIC
    return vrev32q_x(a, p);
#else /* POLYMORPHIC */
    return vrev32q_x_f16(a, p);
#endif /* POLYMORPHIC */
}

// CHECK-LABEL: @test_vrev32q_x_s8(
// CHECK-NEXT:  entry:
// CHECK-NEXT:    [[TMP0:%.*]] = zext i16 [[P:%.*]] to i32
// CHECK-NEXT:    [[TMP1:%.*]] = call <16 x i1> @llvm.arm.mve.pred.i2v.v16i1(i32 [[TMP0]])
// CHECK-NEXT:    [[TMP2:%.*]] = call <16 x i8> @llvm.arm.mve.vrev.predicated.v16i8.v16i1(<16 x i8> [[A:%.*]], i32 32, <16 x i1> [[TMP1]], <16 x i8> undef)
// CHECK-NEXT:    ret <16 x i8> [[TMP2]]
//
int8x16_t test_vrev32q_x_s8(int8x16_t a, mve_pred16_t p)
{
#ifdef POLYMORPHIC
    return vrev32q_x(a, p);
#else /* POLYMORPHIC */
    return vrev32q_x_s8(a, p);
#endif /* POLYMORPHIC */
}

// CHECK-LABEL: @test_vrev32q_x_s16(
// CHECK-NEXT:  entry:
// CHECK-NEXT:    [[TMP0:%.*]] = zext i16 [[P:%.*]] to i32
// CHECK-NEXT:    [[TMP1:%.*]] = call <8 x i1> @llvm.arm.mve.pred.i2v.v8i1(i32 [[TMP0]])
// CHECK-NEXT:    [[TMP2:%.*]] = call <8 x i16> @llvm.arm.mve.vrev.predicated.v8i16.v8i1(<8 x i16> [[A:%.*]], i32 32, <8 x i1> [[TMP1]], <8 x i16> undef)
// CHECK-NEXT:    ret <8 x i16> [[TMP2]]
//
int16x8_t test_vrev32q_x_s16(int16x8_t a, mve_pred16_t p)
{
#ifdef POLYMORPHIC
    return vrev32q_x(a, p);
#else /* POLYMORPHIC */
    return vrev32q_x_s16(a, p);
#endif /* POLYMORPHIC */
}

// CHECK-LABEL: @test_vrev32q_x_u8(
// CHECK-NEXT:  entry:
// CHECK-NEXT:    [[TMP0:%.*]] = zext i16 [[P:%.*]] to i32
// CHECK-NEXT:    [[TMP1:%.*]] = call <16 x i1> @llvm.arm.mve.pred.i2v.v16i1(i32 [[TMP0]])
// CHECK-NEXT:    [[TMP2:%.*]] = call <16 x i8> @llvm.arm.mve.vrev.predicated.v16i8.v16i1(<16 x i8> [[A:%.*]], i32 32, <16 x i1> [[TMP1]], <16 x i8> undef)
// CHECK-NEXT:    ret <16 x i8> [[TMP2]]
//
uint8x16_t test_vrev32q_x_u8(uint8x16_t a, mve_pred16_t p)
{
#ifdef POLYMORPHIC
    return vrev32q_x(a, p);
#else /* POLYMORPHIC */
    return vrev32q_x_u8(a, p);
#endif /* POLYMORPHIC */
}

// CHECK-LABEL: @test_vrev32q_x_u16(
// CHECK-NEXT:  entry:
// CHECK-NEXT:    [[TMP0:%.*]] = zext i16 [[P:%.*]] to i32
// CHECK-NEXT:    [[TMP1:%.*]] = call <8 x i1> @llvm.arm.mve.pred.i2v.v8i1(i32 [[TMP0]])
// CHECK-NEXT:    [[TMP2:%.*]] = call <8 x i16> @llvm.arm.mve.vrev.predicated.v8i16.v8i1(<8 x i16> [[A:%.*]], i32 32, <8 x i1> [[TMP1]], <8 x i16> undef)
// CHECK-NEXT:    ret <8 x i16> [[TMP2]]
//
uint16x8_t test_vrev32q_x_u16(uint16x8_t a, mve_pred16_t p)
{
#ifdef POLYMORPHIC
    return vrev32q_x(a, p);
#else /* POLYMORPHIC */
    return vrev32q_x_u16(a, p);
#endif /* POLYMORPHIC */
}

// CHECK-LABEL: @test_vrev64q_x_f16(
// CHECK-NEXT:  entry:
// CHECK-NEXT:    [[TMP0:%.*]] = zext i16 [[P:%.*]] to i32
// CHECK-NEXT:    [[TMP1:%.*]] = call <8 x i1> @llvm.arm.mve.pred.i2v.v8i1(i32 [[TMP0]])
// CHECK-NEXT:    [[TMP2:%.*]] = call <8 x half> @llvm.arm.mve.vrev.predicated.v8f16.v8i1(<8 x half> [[A:%.*]], i32 64, <8 x i1> [[TMP1]], <8 x half> undef)
// CHECK-NEXT:    ret <8 x half> [[TMP2]]
//
float16x8_t test_vrev64q_x_f16(float16x8_t a, mve_pred16_t p)
{
#ifdef POLYMORPHIC
    return vrev64q_x(a, p);
#else /* POLYMORPHIC */
    return vrev64q_x_f16(a, p);
#endif /* POLYMORPHIC */
}

// CHECK-LABEL: @test_vrev64q_x_f32(
// CHECK-NEXT:  entry:
// CHECK-NEXT:    [[TMP0:%.*]] = zext i16 [[P:%.*]] to i32
// CHECK-NEXT:    [[TMP1:%.*]] = call <4 x i1> @llvm.arm.mve.pred.i2v.v4i1(i32 [[TMP0]])
// CHECK-NEXT:    [[TMP2:%.*]] = call <4 x float> @llvm.arm.mve.vrev.predicated.v4f32.v4i1(<4 x float> [[A:%.*]], i32 64, <4 x i1> [[TMP1]], <4 x float> undef)
// CHECK-NEXT:    ret <4 x float> [[TMP2]]
//
float32x4_t test_vrev64q_x_f32(float32x4_t a, mve_pred16_t p)
{
#ifdef POLYMORPHIC
    return vrev64q_x(a, p);
#else /* POLYMORPHIC */
    return vrev64q_x_f32(a, p);
#endif /* POLYMORPHIC */
}

// CHECK-LABEL: @test_vrev64q_x_s8(
// CHECK-NEXT:  entry:
// CHECK-NEXT:    [[TMP0:%.*]] = zext i16 [[P:%.*]] to i32
// CHECK-NEXT:    [[TMP1:%.*]] = call <16 x i1> @llvm.arm.mve.pred.i2v.v16i1(i32 [[TMP0]])
// CHECK-NEXT:    [[TMP2:%.*]] = call <16 x i8> @llvm.arm.mve.vrev.predicated.v16i8.v16i1(<16 x i8> [[A:%.*]], i32 64, <16 x i1> [[TMP1]], <16 x i8> undef)
// CHECK-NEXT:    ret <16 x i8> [[TMP2]]
//
int8x16_t test_vrev64q_x_s8(int8x16_t a, mve_pred16_t p)
{
#ifdef POLYMORPHIC
    return vrev64q_x(a, p);
#else /* POLYMORPHIC */
    return vrev64q_x_s8(a, p);
#endif /* POLYMORPHIC */
}

// CHECK-LABEL: @test_vrev64q_x_s16(
// CHECK-NEXT:  entry:
// CHECK-NEXT:    [[TMP0:%.*]] = zext i16 [[P:%.*]] to i32
// CHECK-NEXT:    [[TMP1:%.*]] = call <8 x i1> @llvm.arm.mve.pred.i2v.v8i1(i32 [[TMP0]])
// CHECK-NEXT:    [[TMP2:%.*]] = call <8 x i16> @llvm.arm.mve.vrev.predicated.v8i16.v8i1(<8 x i16> [[A:%.*]], i32 64, <8 x i1> [[TMP1]], <8 x i16> undef)
// CHECK-NEXT:    ret <8 x i16> [[TMP2]]
//
int16x8_t test_vrev64q_x_s16(int16x8_t a, mve_pred16_t p)
{
#ifdef POLYMORPHIC
    return vrev64q_x(a, p);
#else /* POLYMORPHIC */
    return vrev64q_x_s16(a, p);
#endif /* POLYMORPHIC */
}

// CHECK-LABEL: @test_vrev64q_x_s32(
// CHECK-NEXT:  entry:
// CHECK-NEXT:    [[TMP0:%.*]] = zext i16 [[P:%.*]] to i32
// CHECK-NEXT:    [[TMP1:%.*]] = call <4 x i1> @llvm.arm.mve.pred.i2v.v4i1(i32 [[TMP0]])
// CHECK-NEXT:    [[TMP2:%.*]] = call <4 x i32> @llvm.arm.mve.vrev.predicated.v4i32.v4i1(<4 x i32> [[A:%.*]], i32 64, <4 x i1> [[TMP1]], <4 x i32> undef)
// CHECK-NEXT:    ret <4 x i32> [[TMP2]]
//
int32x4_t test_vrev64q_x_s32(int32x4_t a, mve_pred16_t p)
{
#ifdef POLYMORPHIC
    return vrev64q_x(a, p);
#else /* POLYMORPHIC */
    return vrev64q_x_s32(a, p);
#endif /* POLYMORPHIC */
}

// CHECK-LABEL: @test_vrev64q_x_u8(
// CHECK-NEXT:  entry:
// CHECK-NEXT:    [[TMP0:%.*]] = zext i16 [[P:%.*]] to i32
// CHECK-NEXT:    [[TMP1:%.*]] = call <16 x i1> @llvm.arm.mve.pred.i2v.v16i1(i32 [[TMP0]])
// CHECK-NEXT:    [[TMP2:%.*]] = call <16 x i8> @llvm.arm.mve.vrev.predicated.v16i8.v16i1(<16 x i8> [[A:%.*]], i32 64, <16 x i1> [[TMP1]], <16 x i8> undef)
// CHECK-NEXT:    ret <16 x i8> [[TMP2]]
//
uint8x16_t test_vrev64q_x_u8(uint8x16_t a, mve_pred16_t p)
{
#ifdef POLYMORPHIC
    return vrev64q_x(a, p);
#else /* POLYMORPHIC */
    return vrev64q_x_u8(a, p);
#endif /* POLYMORPHIC */
}

// CHECK-LABEL: @test_vrev64q_x_u16(
// CHECK-NEXT:  entry:
// CHECK-NEXT:    [[TMP0:%.*]] = zext i16 [[P:%.*]] to i32
// CHECK-NEXT:    [[TMP1:%.*]] = call <8 x i1> @llvm.arm.mve.pred.i2v.v8i1(i32 [[TMP0]])
// CHECK-NEXT:    [[TMP2:%.*]] = call <8 x i16> @llvm.arm.mve.vrev.predicated.v8i16.v8i1(<8 x i16> [[A:%.*]], i32 64, <8 x i1> [[TMP1]], <8 x i16> undef)
// CHECK-NEXT:    ret <8 x i16> [[TMP2]]
//
uint16x8_t test_vrev64q_x_u16(uint16x8_t a, mve_pred16_t p)
{
#ifdef POLYMORPHIC
    return vrev64q_x(a, p);
#else /* POLYMORPHIC */
    return vrev64q_x_u16(a, p);
#endif /* POLYMORPHIC */
}

// CHECK-LABEL: @test_vrev64q_x_u32(
// CHECK-NEXT:  entry:
// CHECK-NEXT:    [[TMP0:%.*]] = zext i16 [[P:%.*]] to i32
// CHECK-NEXT:    [[TMP1:%.*]] = call <4 x i1> @llvm.arm.mve.pred.i2v.v4i1(i32 [[TMP0]])
// CHECK-NEXT:    [[TMP2:%.*]] = call <4 x i32> @llvm.arm.mve.vrev.predicated.v4i32.v4i1(<4 x i32> [[A:%.*]], i32 64, <4 x i1> [[TMP1]], <4 x i32> undef)
// CHECK-NEXT:    ret <4 x i32> [[TMP2]]
//
uint32x4_t test_vrev64q_x_u32(uint32x4_t a, mve_pred16_t p)
{
#ifdef POLYMORPHIC
    return vrev64q_x(a, p);
#else /* POLYMORPHIC */
    return vrev64q_x_u32(a, p);
#endif /* POLYMORPHIC */
}
