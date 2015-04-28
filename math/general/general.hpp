#ifndef GENERAL_INCLUDED_
#define GENERAL_INCLUDED_


#include <cmath>

#ifdef CAFF_MATH_SIMD
#include <xmmintrin.h>
#endif


namespace caffeine {
namespace math {


float sqrt(const float x)
{
	#ifdef CAFF_MATH_SIMD

	__m128 val = _mm_load_ps1(&x);
	_mm_sqrt_ss(val);

	return val[0];

	#else

	return std::sqrt(x);

	#endif
}


} // namespace
} // namespace


#endif // include guard