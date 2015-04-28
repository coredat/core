#ifndef VECTOR4_INCLUDED_
#define VECTOR4_INCLUDED_



#include "vector_types.hpp"
#include "../general/general.hpp"
#include <array>
#include <string>
#include <assert.h>
#include <iostream>

namespace caffmath = ::caffeine::math;

namespace caffeine {
namespace math {


// ** Interface ** //


// Initialize vector.
inline vector4 vector4_init(const float val);
inline vector4 vector4_init(const float x, const float y, const float z, const float w);

// Get components.
inline float vector4_get_x(const vector4 vec);
inline float vector4_get_y(const vector4 vec);
inline float vector4_get_z(const vector4 vec);
inline float vector4_get_w(const vector4 vec);

// Component wise arithmetic.
inline vector4 vector4_add(const vector4 a, const vector4 b);
inline vector4 vector4_subtract(const vector4 a, const vector4 b);
inline vector4 vector4_multiply(const vector4 a, const vector4 b);
inline vector4 vector4_divide(const vector4 a, const vector4 b);

// Special operations.
inline vector4 vector4_scale(const vector4 a, const float scale);
inline vector4 vector4_normalize(const vector4 a);
inline float vector4_length(const vector4 a);

// To other formats.
inline void vector4_to_array(const vector4 a, float *out_array);
inline std::array<float, 4> vector4_to_std_array(const vector4 a);
inline std::string vector4_to_std_string(const vector4 a);


// ** IMPL ** //

// This is not part of public interface, keep walking :)
// In seriousness anything inside this namespace is prone to change and should
// be considered private to the implimentation of the vector.
namespace detail
{
	struct internal_vec4
	{
		float x, y, z, w;
	};
}

vector4 vector4_init(const float val)
{
	#ifdef CAFF_MATH_SIMD

	return _mm_load_ps1(&val);

	#else

	return vector4_init(val, val, val, val);

	#endif
}


vector4 vector4_init(const float x, const float y, const float z, const float w)
{
	#ifdef CAFF_MATH_SIMD

	float array[] = {x, y, z, w};
	const vector4 return_vec = _mm_load_ps(array);
 
	#else

	vector4 return_vec;
	detail::internal_vec4 *internal_vec = reinterpret_cast<detail::internal_vec4*>(&return_vec);

	internal_vec->x = x;
	internal_vec->y = y;
	internal_vec->z = z;
	internal_vec->w = w;

	#endif

	return return_vec;
}


float vector4_get_x(const vector4 vec)
{
	#ifdef CAFF_MATH_SIMD

	return vec[0];

	#else

	const detail::internal_vec4 *internal_vec = reinterpret_cast<const detail::internal_vec4*>(&vec);
	return internal_vec->x;

	#endif
}


float vector4_get_y(const vector4 vec)
{
	#ifdef CAFF_MATH_SIMD

	return vec[1];

	#else

	const detail::internal_vec4 *internal_vec = reinterpret_cast<const detail::internal_vec4*>(&vec);
	return internal_vec->y;

	#endif
}


float vector4_get_z(const vector4 vec)
{
	#ifdef CAFF_MATH_SIMD

	return vec[2];

	#else

	const detail::internal_vec4 *internal_vec = reinterpret_cast<const detail::internal_vec4*>(&vec);
	return internal_vec->z;

	#endif
}


float vector4_get_w(const vector4 vec)
{
	#ifdef CAFF_MATH_SIMD

	return vec[3];

	#else

	const detail::internal_vec4 *internal_vec = reinterpret_cast<const detail::internal_vec4*>(&vec);
	return internal_vec->w;

	#endif
}


vector4 vector4_add(const vector4 a, const vector4 b)
{
	#ifdef CAFF_MATH_SIMD

	return _mm_add_ps(a, b);

	#else

	const detail::internal_vec4 *vec_a = reinterpret_cast<const detail::internal_vec4*>(&a);
	const detail::internal_vec4 *vec_b = reinterpret_cast<const detail::internal_vec4*>(&b);

	return vector4_init(vec_a->x + vec_b->x, vec_a->y + vec_b->y, vec_a->z + vec_b->z, vec_a->w + vec_b->w);

	#endif
}


vector4 vector4_subtract(const vector4 a, const vector4 b)
{
	#ifdef CAFF_MATH_SIMD

	return _mm_sub_ps(a, b);

	#else

	const detail::internal_vec4 *vec_a = reinterpret_cast<const detail::internal_vec4*>(&a);
	const detail::internal_vec4 *vec_b = reinterpret_cast<const detail::internal_vec4*>(&b);

	return vector4_init(vec_a->x - vec_b->x, vec_a->y - vec_b->y, vec_a->z - vec_b->z, vec_a->w - vec_b->w);

	#endif
}


vector4 vector4_multiply(const vector4 a, const vector4 b)
{
	#ifdef CAFF_MATH_SIMD

	return _mm_mul_ps(a, b);

	#else

	const detail::internal_vec4 *vec_a = reinterpret_cast<const detail::internal_vec4*>(&a);
	const detail::internal_vec4 *vec_b = reinterpret_cast<const detail::internal_vec4*>(&b);

	return vector4_init(vec_a->x * vec_b->x, vec_a->y * vec_b->y, vec_a->z * vec_b->z, vec_a->w * vec_b->w);

	#endif
}


vector4 vector4_divide(const vector4 a, const vector4 b)
{
	#ifdef CAFF_MATH_SIMD

	return _mm_div_ps(a, b);

	#else

	const detail::internal_vec4 *vec_a = reinterpret_cast<const detail::internal_vec4*>(&a);
	const detail::internal_vec4 *vec_b = reinterpret_cast<const detail::internal_vec4*>(&b);

	// Divide by zero check.
	assert(vec_b->x != 0 && vec_b->y != 0 && vec_b->z != 0 && vec_b->w != 0);

	return vector4_init(vec_a->x / vec_b->x, vec_a->y / vec_b->y, vec_a->z / vec_b->z, vec_a->w / vec_b->w);

	#endif
}


vector4 vector4_scale(const vector4 a, const float scale)
{
	const vector4 scale_vec = vector4_init(scale);
	return vector4_multiply(a, scale_vec);
}


vector4 vector4_normalize(const vector4 a)
{
	const float length = vector4_length(a);

	// Don't pass zero vectors.
	assert(length != 0);

	return caffmath::vector4_scale(a, length);
}


float vector4_length(const vector4 a)
{
	const detail::internal_vec4 *vec_a = reinterpret_cast<const detail::internal_vec4*>(&a);
	const float x = vec_a->x * vec_a->x + vec_a->y * vec_a->y + vec_a->z * vec_a->z + vec_a->w * vec_a->w;

	return caffmath::sqrt(x);
}


} // namespace
} // namespace


#endif // include guard