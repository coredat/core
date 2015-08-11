#ifndef MATRIX44_INCLUDED_45952EA8_98A0_40E4_8677_AE79A577E975
#define MATRIX44_INCLUDED_45952EA8_98A0_40E4_8677_AE79A577E975


/*
  Matrix 44
  Through out this library we consider a 4x4 matrix as a 
  world matrix.
*/


#include "mat_types.hpp"
#include "../vec/vec4.hpp"
#include <array>
#include <cstring>


namespace math {


// Constants
inline mat4                       mat4_id();
inline mat4                       mat4_zero();

// Init
inline mat4                       mat4_init(); // will return an id array
inline mat4                       mat4_init(const float x);
inline mat4                       mat4_init_with_array(const float arr[]);
inline mat4                       mat4_init_with_array(const std::array<float, 16> &array);

// Generate transformation matrices.
inline mat4                       mat4_lookat(const vec3 eye_position, const vec3 look_at_position, const vec3 up);
inline mat4                       mat4_projection(const float width, const float height, const float near_plane, const float far_plane, const float fov);
inline mat4                       mat4_orthographic(const float width, const float height, const float depth); // Not impl
inline mat4                       mat4_scale(const float x, const float y, const float z);
inline mat4                       mat4_translate(const vec3 move);
inline mat4                       mat4_translate(const float x, const float y, const float z);
inline mat4                       mat4_rotate_around_axis(const vec3 axis, const float radians);

// Operations
inline mat4                       mat4_add(const mat4 &lhs, const mat4 &rhs);
inline mat4                       mat4_subtract(const mat4 &lhs, const mat4 &rhs);
inline mat4                       mat4_multiply(const float val, const mat4 &b);
inline vec4                       mat4_multiply(const vec4 vec, const mat4 &b);
inline mat4                       mat4_multiply(const mat4 &lhs, const mat4 &rhs);

// Transform matrices into other forms
inline mat4                       mat4_get_transpose(const mat4 &a);
inline mat4                       mat4_get_inverse(const mat4 &a);
inline mat4                       mat4_get_scale(const mat4 &a, const vec3 scale);
inline void                       mat4_to_array(const mat4 &m, float *array);
inline std::array<float, 16>      mat4_to_array(const mat4 &m);

// Get/Set information
inline float                      mat4_get(const mat4 &mat, const uint32_t row, const uint32_t col);
inline float                      mat4_get(const mat4 &mat, const uint32_t i);
inline const float*               mat4_get_data(const mat4 &mat);
inline void                       mat4_set(mat4 &mat, const uint32_t row, const uint32_t col, const float set);
inline mat3                       mat4_get_rotation(const mat4 &a);
inline vec3                       mat4_get_position(const mat4 &a);
inline vec3                       mat4_get_scale(const mat4 &a);


// Impl

namespace detail
{
  struct internal_mat4
  {
    float data[16];
  };

}


mat4
mat4_id()
{
  std::array<float, 16> id_array = {
    1.f, 0.f, 0.f, 0.f,
    0.f, 1.f, 0.f, 0.f,
    0.f, 0.f, 1.f, 0.f,
    0.f, 0.f, 0.f, 1.f,
  };

  return mat4_init_with_array(&id_array[0]);
}


mat4
mat4_zero()
{
  std::array<float, 16> zero_array = {
    0.f, 0.f, 0.f, 0.f,
    0.f, 0.f, 0.f, 0.f,
    0.f, 0.f, 0.f, 0.f,
    0.f, 0.f, 0.f, 0.f,
  };

  return mat4_init_with_array(&zero_array[0]);  
}


mat4
mat4_init()
{
  return mat4_id();
}


mat4
mat4_init(const float x)
{
  std::array<float, 16> x_array = {
    x,x,x,x,
    x,x,x,x,
    x,x,x,x,
  };

  return mat4_init_with_array(x_array);
}


mat4
mat4_init_with_array(const float array[])
{
  mat4 return_mat;
  detail::internal_mat4 *internal_mat = reinterpret_cast<detail::internal_mat4*>(&return_mat);

  memcpy(internal_mat->data, array, sizeof(internal_mat->data));
  
  return return_mat;
}


mat4
mat4_init_with_array(const std::array<float, 16> &array)
{
  return mat4_init_with_array(&array[0]);
}


mat4
mat4_lookat(const vec3 eye_position, const vec3 look_at_position, const vec3 up)
{
  const vec3 z_axis = vec3_normalize(vec3_subtract(look_at_position, eye_position));
  const vec3 x_axis = vec3_normalize(vec3_cross(z_axis, up));
  const vec3 y_axis = vec3_cross(x_axis, z_axis);

  const std::array<float, 16> array_mat =
  {
    vec3_get_x(x_axis),
    vec3_get_x(y_axis),
    -vec3_get_x(z_axis),
    0.f,

    vec3_get_y(x_axis),
    vec3_get_y(y_axis),
    -vec3_get_y(z_axis),
    0.f,

    vec3_get_z(x_axis),
    vec3_get_z(y_axis),
    -vec3_get_z(z_axis),
    0.f,

    -(vec3_dot(x_axis, eye_position)),
    -(vec3_dot(y_axis, eye_position)),
    +(vec3_dot(z_axis, eye_position)),
    1.f,
  };

  return mat4_init_with_array(&array_mat[0]);
}


mat4
mat4_projection(const float width, const float height, const float near_plane, const float far_plane, const float fov)
{
  const float aspect_ratio = width / height;
  const float one_over_tan_half_fov = 1.f / math::tan(fov * 0.5f);
  const float plane_diff = far_plane - near_plane;

  //mat4 return_mat = mat4_zero();
  std::array<float, 16> proj_mat =
  {
    one_over_tan_half_fov / aspect_ratio,
    0.f,
    0.f,
    0.f,

    0.f,
    one_over_tan_half_fov,
    0.f,
    0.f, 

    0.f,
    0.f,
    -(far_plane + near_plane) / plane_diff,
    -1.f,

    0.f,
    0.f,
    -(2.f * far_plane * near_plane) / plane_diff,
    0.f,

 };

 return mat4_init_with_array(&proj_mat[0]);
}


mat4
mat4_orthographic(const float width, const float height, const float depth)
{
  return mat4_zero();
}


// Operations
mat4
mat4_add(const mat4 &lhs, const mat4 &rhs)
{
  const detail::internal_mat4 *left = reinterpret_cast<const detail::internal_mat4*>(&lhs);
  const detail::internal_mat4 *right = reinterpret_cast<const detail::internal_mat4*>(&rhs);

  mat4 return_mat; 
  detail::internal_mat4 *internal_mat = reinterpret_cast<detail::internal_mat4*>(&return_mat);

  for(int i = 0; i < 16; ++i)
  {
    internal_mat->data[i] = left->data[0] + right->data[0];
  }

  return return_mat;
}


mat4
mat4_subtract(const mat4 &lhs, const mat4 &rhs)
{
  const detail::internal_mat4 *left = reinterpret_cast<const detail::internal_mat4*>(&lhs);
  const detail::internal_mat4 *right = reinterpret_cast<const detail::internal_mat4*>(&rhs);

  mat4 return_mat; 
  detail::internal_mat4 *internal_mat = reinterpret_cast<detail::internal_mat4*>(&return_mat);

  for(int i = 0; i < 16; ++i)
  {
    internal_mat->data[i] = left->data[0] - right->data[0];
  }

  return return_mat;
}


mat4
mat4_multiply(const float lhs, const mat4 &rhs)
{
  mat4 multiply_mat = mat4_init(lhs);
  
  return mat4_multiply(multiply_mat, rhs);
}
  
  
mat4
mat4_scale(const float x, const float y, const float z)
{
  mat4 return_mat = mat4_id();
  detail::internal_mat4 *internal_mat = reinterpret_cast<detail::internal_mat4*>(&return_mat);
  
  internal_mat->data[0]  = x;
  internal_mat->data[5]  = y;
  internal_mat->data[10] = z;
  internal_mat->data[15] = 1;
  
  return return_mat;
}


vec4
mat4_multiply(const vec4 lhs, const mat4 &rhs)
{
  const detail::internal_mat4 *right = reinterpret_cast<const detail::internal_mat4*>(&rhs);
  std::array<float, 4> vec_data;

  for(int i = 0; i < 16; i += 4)
  {
    const vec4 dot_vec = vec4_init(right->data[i + 0], right->data[i + 4], right->data[i + 8], right->data[i + 12]);

    vec_data.at(i / 4) = vec4_dot(lhs, dot_vec);
  }

  return vec4_init_with_array(vec_data);
}


mat4
mat4_multiply(const mat4 &lhs, const mat4 &rhs)
{
  const detail::internal_mat4 *left = reinterpret_cast<const detail::internal_mat4*>(&lhs);
  const detail::internal_mat4 *right = reinterpret_cast<const detail::internal_mat4*>(&rhs);

  mat4 return_mat = mat4_id();
  detail::internal_mat4 *internal_mat = reinterpret_cast<detail::internal_mat4*>(&return_mat);

  for(uint32_t i = 0; i < 16; ++i)
  {
    // Starting index for data.
    const uint32_t row = (i / 4) * 4;
    const uint32_t col = (i % 4);

    const vec4 row_vec = vec4_init(left->data[row + 0],  left->data[row + 1],  left->data[row + 2],  left->data[row + 3]);
    const vec4 col_vec = vec4_init(right->data[col + 0], right->data[col + 4], right->data[col + 8], right->data[col + 12]);

    const float dot = vec4_dot(row_vec, col_vec);
    
    internal_mat->data[i] = dot;
  }

  return return_mat;
}


mat4
mat4_translate(const vec3 move)
{
  return mat4_translate(vec3_get_x(move), vec3_get_y(move), vec3_get_z(move));
}


mat4
mat4_translate(const float x, const float y, const float z)
{
  mat4 translate_mat = mat4_id();
  detail::internal_mat4 *translate_me = reinterpret_cast<detail::internal_mat4*>(&translate_mat);

  translate_me->data[12] += x;
  translate_me->data[13] += y;
  translate_me->data[14] += z;

  return translate_mat;
}
  
  
mat4
mat4_rotate_around_axis(const vec3 axis, const float radians)
{
  mat4 rotation = mat4_id();
  detail::internal_mat4 *rotate_me = reinterpret_cast<detail::internal_mat4*>(&rotation);
  
  const float sin_theta           = math::sin(radians);
  const float cos_theta           = math::cos(radians);
  const float one_minus_cos_theta = 1.f - cos_theta;
  
  const float x = vec3_get_x(axis);
  const float y = vec3_get_y(axis);
  const float z = vec3_get_z(axis);
  
  rotate_me->data[0]  = cos_theta + ((x * x) * one_minus_cos_theta);
  rotate_me->data[1]  = ((x * y) * one_minus_cos_theta) - (z * sin_theta);
  rotate_me->data[2]  = ((x * z) * one_minus_cos_theta) + (y * sin_theta);
  
  rotate_me->data[4]  = ((y * x) * one_minus_cos_theta) + (z * sin_theta);
  rotate_me->data[5]  = cos_theta + ((y * y) * one_minus_cos_theta);
  rotate_me->data[6]  = ((y * z) * one_minus_cos_theta) - (x * sin_theta);

  rotate_me->data[8]  = ((z * x) * one_minus_cos_theta) - (y * sin_theta);
  rotate_me->data[9]  = ((z * y) * one_minus_cos_theta) + (x * sin_theta);
  rotate_me->data[10] = cos_theta + ((z * z) * one_minus_cos_theta);
  
  rotate_me->data[15] = 1.f;
  
  return rotation;
}


float
mat4_get(const mat4 &mat, const uint32_t row, const uint32_t col)
{
  assert(row < 4 && col < 4);

  const uint32_t i = (row * 4) + col;
  return mat4_get(mat, i);
}


float
mat4_get(const mat4 &mat, const uint32_t index)
{
  assert(index < 16);
  
  const detail::internal_mat4 *internal_mat = reinterpret_cast<const detail::internal_mat4*>(&mat);
  return internal_mat->data[index];
}

  
const float*
mat4_get_data(const mat4 &mat)
{
  const detail::internal_mat4 *internal_mat = reinterpret_cast<const detail::internal_mat4*>(&mat);
  return &(internal_mat->data[0]);
}

  
void
mat4_set(mat4 &mat, const uint32_t row, const uint32_t col, const float set)
{
  assert(row < 4 && col < 4);

  detail::internal_mat4 *internal_mat = reinterpret_cast<detail::internal_mat4*>(&mat);
  internal_mat->data[(row * 4) + col] = set;
}


} // namespace


#endif // include guard