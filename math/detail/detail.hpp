#ifndef DETAIL_INCLUDED_738ADE13_1093_4593_AF81_2F9CB58DAE42
#define DETAIL_INCLUDED_738ADE13_1093_4593_AF81_2F9CB58DAE42


#ifdef MATH_ENABLE_SIMD
#define MATH_NO_SIMD
#define MATH_SSE2
#endif


#ifdef MATH_FORCE_INLINE
#define MATH_INLINE __forceinline
#else
#define MATH_INLINE inline
#endif


#define MATH_CONST __attribute__((const))

#define MATH_LIKELY(x)       __builtin_expect((x),1)
#define MATH_UNLIKELY(x)     __builtin_expect((x),0)


#define MATH_VEC2_INLINE inline
#define MATH_VEC3_INLINE inline
#define MATH_VEC4_INLINE inline
#define MATH_MAT3_INLINE inline
#define MATH_MAT4_INLINE inline
#define MATH_QUAT_INLINE inline
#define MATH_GENR_INLINE inline


#endif // include guard
