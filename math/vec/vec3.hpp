#ifndef VEC3_INCLUDED_DD26787F_2663_4F50_85D0_7188D23BA64B
#define VEC3_INCLUDED_DD26787F_2663_4F50_85D0_7188D23BA64B


/*
  Vector3
  3D vector interface and impl.
*/


#include "vec_types.hpp"
#include "vec2.hpp"
#include "../general/general.hpp"
#include <array>
#include <assert.h>


namespace math {


// ** Interface ** //

// Constants
inline vec3                   vec3_zero();
inline vec3                   vec3_one();
inline vec3                   vec3_zero_zero_one();

// Initialize vector.
inline vec3                   vec3_init(const float val);
inline vec3                   vec3_init(const float x, const float y, const float z);
inline vec3                   vec3_init(const vec2 vec, const float z);
inline vec3                   vec3_init_with_array(const float *arr);
inline vec3                   vec3_init_with_array(const std::array<float, 3> &vec);

// Get components.
inline float                  vec3_get_x(const vec3 vec);
inline float                  vec3_get_y(const vec3 vec);
inline float                  vec3_get_z(const vec3 vec);
inline void                   vec3_to_array(const vec3 a, float *out_array);
inline std::array<float, 3>   vec3_to_std_array(const vec3 a);

// Component wise arithmetic.
inline vec3                   vec3_add(const vec3 a, const vec3 b);
inline vec3                   vec3_subtract(const vec3 a, const vec3 b);
inline vec3                   vec3_multiply(const vec3 a, const vec3 b);
inline vec3                   vec3_divide(const vec3 a, const vec3 b);

// Special operations.
inline vec3                   vec3_lerp(const vec3 start, const vec3 end, const float dt);
//inline vec3                vec3_slerp(const vec3 start, const vec3 end, const float dt);
inline vec3                   vec3_scale(const vec3 a, const float scale);
inline vec3                   vec3_normalize(const vec3 a);
inline float                  vec3_length(const vec3 a);
inline vec3                   vec3_cross(const vec3 a, const vec3 b); // not impl
inline float                  vec3_dot(const vec3 a, const vec3 b); // not impl


// ** Implimentation ** //

// This is not part of public interface, keep walking :)
// Using anything in the detail namespace is undefined-behaviour.
namespace detail
{
  struct internal_vec3
  {
    float x, y, z;
  };
}


// Constants
vec3
vec3_zero()
{
  return vec3_init(0.f);
}


vec3
vec3_one()
{
  return vec3_init(1.f);
}


vec3
vec3_zero_zero_one()
{
  return vec3_init(0.f, 0.f, 1.f);
}

// Initialize

vec3
vec3_init(const float val)
{
  return vec3_init(val, val, val);
}


vec3
vec3_init(const float x, const float y, const float z)
{
  vec3 return_vec;
  detail::internal_vec3 *internal_vec = reinterpret_cast<detail::internal_vec3*>(&return_vec);

  internal_vec->x = x;
  internal_vec->y = y;
  internal_vec->z = z;

  return return_vec;
}


vec3
vec3_init(const vec2 vec, const float z)
{
  return vec3_init(vec2_get_x(vec), vec2_get_y(vec), z);
}


vec3
vec3_init_with_array(const float *arr)
{
  return vec3_init(arr[0], arr[1], arr[2]);
}


vec3
vec3_init_with_array(const std::array<float, 3> &arr)
{
  return vec3_init(arr.at(0), arr.at(1), arr.at(2));
}


// Get components.

float
vec3_get_x(const vec3 vec)
{
  const detail::internal_vec3 *internal_vec = reinterpret_cast<const detail::internal_vec3*>(&vec);
  return internal_vec->x;
}


float
vec3_get_y(const vec3 vec)
{
  const detail::internal_vec3 *internal_vec = reinterpret_cast<const detail::internal_vec3*>(&vec);
  return internal_vec->y;
}


float
vec3_get_z(const vec3 vec)
{
  const detail::internal_vec3 *internal_vec = reinterpret_cast<const detail::internal_vec3*>(&vec);
  return internal_vec->z;
}


void
vec3_to_array(const vec3 a, float *out_array)
{
  out_array[0] = vec3_get_x(a);
  out_array[1] = vec3_get_y(a);
  out_array[2] = vec3_get_z(a);
}


std::array<float, 3>
vec3_to_std_array(const vec3 a)
{
  std::array<float, 3> return_array = {
    vec3_get_x(a),
    vec3_get_y(a),
    vec3_get_z(a),
  };

  return return_array;
}


vec3
vec3_add(const vec3 a, const vec3 b)
{
  const detail::internal_vec3 *vec_a = reinterpret_cast<const detail::internal_vec3*>(&a);
  const detail::internal_vec3 *vec_b = reinterpret_cast<const detail::internal_vec3*>(&b);

  return vec3_init(vec_a->x + vec_b->x, vec_a->y + vec_b->y, vec_a->z + vec_b->z);
}


vec3
vec3_subtract(const vec3 a, const vec3 b)
{
  const detail::internal_vec3 *vec_a = reinterpret_cast<const detail::internal_vec3*>(&a);
  const detail::internal_vec3 *vec_b = reinterpret_cast<const detail::internal_vec3*>(&b);

  return vec3_init(vec_a->x - vec_b->x, vec_a->y - vec_b->y, vec_a->z - vec_b->z);
}


vec3
vec3_multiply(const vec3 a, const vec3 b)
{
  const detail::internal_vec3 *vec_a = reinterpret_cast<const detail::internal_vec3*>(&a);
  const detail::internal_vec3 *vec_b = reinterpret_cast<const detail::internal_vec3*>(&b);

  return vec3_init(vec_a->x * vec_b->x, vec_a->y * vec_b->y, vec_a->z * vec_b->z);
}


vec3
vec3_divide(const vec3 a, const vec3 b)
{
  const detail::internal_vec3 *vec_a = reinterpret_cast<const detail::internal_vec3*>(&a);
  const detail::internal_vec3 *vec_b = reinterpret_cast<const detail::internal_vec3*>(&b);

  // Divide by zero check.
  assert(vec_b->x != 0 && vec_b->y != 0 && vec_b->z != 0);

  return vec3_init(vec_a->x / vec_b->x, vec_a->y / vec_b->y, vec_a->z / vec_b->z);
}


vec3
vec3_lerp(const vec3 start, const vec3 end, const float dt)
{
  const vec3 difference = vec3_subtract(end, start);
  const vec3 scaled     = vec3_scale(difference, dt);
  const vec3 position   = vec3_add(start, scaled);

  return position;
}


// vec3
// vec3_slerp(const vec3 start, const vec3 end, const float dt)
// {
//   const float dot                   = vec3_dot(start, end);
//   const float clamp                 = caffmath::clamp(dot, -1.f, 1.f);
//   const float theta                 = caffmath::a_cos(clamp) * dt;

//   const vec3 start_scale         = vec3_scale(start, dot);
//   const vec3 relative            = vec3_subtract(end, start_scale);
//   const vec3 normal              = vec3_normalize(relative);

//   const float cos_theta             = caffmath::cos(theta);
//   const float sin_theta             = caffmath::sin(theta);

//   const vec3 start_cos_scale     = vec3_scale(start, cos_theta);
//   const vec3 relative_sin_scale  = vec3_scale(normal, sin_theta);

//   return vec3_add(start_cos_scale, relative_sin_scale);
// }


vec3
vec3_scale(const vec3 a, const float scale)
{
  const vec3 scale_vec = vec3_init(scale);
  return vec3_multiply(a, scale_vec);
}


vec3
vec3_normalize(const vec3 a)
{
  const float length = vec3_length(a);

  assert(length != 0); // Don't pass zero vectors. (0,0,0);

  return vec3_scale(a, (1.f / length));
}


float
vec3_length(const vec3 a)
{
  const detail::internal_vec3 *vec_a = reinterpret_cast<const detail::internal_vec3*>(&a);
  const float x = vec_a->x * vec_a->x + vec_a->y * vec_a->y + vec_a->z * vec_a->z;

  return sqrt(x);
}


vec3
vec3_cross(const vec3 a, const vec3 b)
{
  const float x = (vec3_get_y(a) * vec3_get_z(b)) - (vec3_get_z(a) * vec3_get_y(b));
  const float y = (vec3_get_x(a) * vec3_get_z(b)) - (vec3_get_z(a) * vec3_get_x(b));
  const float z = (vec3_get_x(a) * vec3_get_y(b)) - (vec3_get_y(a) * vec3_get_x(b));

  return vec3_init(x, -y, z);
}


float
vec3_dot(const vec3 a, const vec3 b)
{
  return (vec3_get_x(a) * vec3_get_x(b)) +
         (vec3_get_y(a) * vec3_get_y(b)) +
         (vec3_get_z(a) * vec3_get_z(b));
}


} // namespace


#endif // include guard