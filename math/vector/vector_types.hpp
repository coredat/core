#ifndef VECTOR_TYPES_INCLUDED_
#define VECTOR_TYPES_INCLUDED_


#ifdef CAFF_MATH_SIMD
// Include SIMD
#include <xmmintrin.h>
#endif


namespace caffeine {
namespace math {


// SIMD Types.
#ifdef CAFF_MATH_SIMD

using vector4 = __m128;
using vector3 = __m128;
using vector2 = __m128;

// Non SIMD
#else

// The vector elements are private to keep the interface identical to that of
// their simd counterparts.
// This library is designed with simd in mind and provides a fallback.

class vector4
{
	float x, y, z, w;
};

class vector3
{
	float x, y, z;
};

class vector2
{
	float x, y;
};

#endif



	
} // namespace
} // namespace


#endif // include guard