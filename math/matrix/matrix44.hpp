#ifndef MATRIX44_INCLUDED_45952EA8_98A0_40E4_8677_AE79A577E975
#define MATRIX44_INCLUDED_45952EA8_98A0_40E4_8677_AE79A577E975


#include "matrix_types.hpp"
#include "../vector/vector4.hpp"
#include <array>
#include <cstring>


namespace caffeine {
namespace math {


// Constants
inline matrix44                   matrix44_id();
inline matrix44                   matrix44_zero();

// Init
inline matrix44                   matrix44_init(); // will return an id array
inline matrix44                   matrix44_init(const float x);
inline matrix44                   matrix44_init_with_array(const float arr[]);
inline matrix44                   matrix44_init_with_array(const std::array<float, 16> &array);

// Generate special matrices
inline matrix44                   matrix44_init_lookat(const vector3 eye_position, const vector3 look_at_position, const vector3 up);
inline matrix44                   matrix44_init_projection(const float width, const float height, const float near_plane, const float far_plane, const float fov);
inline matrix44                   matrix44_init_orthographic(const float width, const float height, const float depth);

// Operations
inline matrix44                   matrix44_add(const matrix44 &lhs, const matrix44 &rhs);
inline matrix44                   matrix44_subtract(const matrix44 &lhs, const matrix44 &rhs);
inline matrix44                   matrix44_scale(const float scale, const matrix44 &b);
inline vector4                    matrix44_multiply(const vector4 vector, const matrix44 &b);
inline matrix44                   matrix44_multiply(const matrix44 &lhs, const matrix44 &rhs);
inline matrix44                   matrix44_translate(const matrix44 &lhs, const vector3 move);
inline matrix44                   matrix44_rotate(const matrix44 &lhs, const vector3 euler);

// Transform matrices into other forms
inline matrix44                   matrix44_get_transpose(const matrix44 &a);
inline matrix44                   matrix44_get_inverse(const matrix44 &a);
inline matrix44                   matrix44_get_scale(const matrix44 &a, const vector3 scale);
inline void                       matrix44_to_array(const matrix44 &m, float *array);
inline std::array<float, 16>      matrix44_to_array(const matrix44 &m);

// Get/Set information
inline float                      matrix44_get(const matrix44 &mat, const uint32_t row, const uint32_t col);
inline float                      matrix44_get(const matrix44 &mat, const uint32_t i);
inline void                       matrix44_set(matrix44 &mat, const uint32_t row, const uint32_t col, const float set);
inline matrix33                   matrix44_get_rotation(const matrix44 &a);
inline vector3                    matrix44_get_position(const matrix44 &a);
inline vector3                    matrix44_get_scale(const matrix44 &a);


// Impl

namespace detail
{
  struct internal_mat4
  {
    float data[16];
  };

}


matrix44
matrix44_id()
{
  std::array<float, 16> id_array = {
    1.f, 0.f, 0.f, 0.f,
    0.f, 1.f, 0.f, 0.f,
    0.f, 0.f, 1.f, 0.f,
    0.f, 0.f, 0.f, 1.f,
  };

  return matrix44_init_with_array(&id_array[0]);
}


matrix44
matrix44_zero()
{
  std::array<float, 16> zero_array = {
    0.f, 0.f, 0.f, 0.f,
    0.f, 0.f, 0.f, 0.f,
    0.f, 0.f, 0.f, 0.f,
    0.f, 0.f, 0.f, 0.f,
  };

  return matrix44_init_with_array(&zero_array[0]);  
}


matrix44
matrix44_init()
{
  return matrix44_id();
}


matrix44
matrix44_init(const float x)
{
  std::array<float, 16> x_array = {
    x,x,x,x,
    x,x,x,x,
    x,x,x,x,
  };

  return matrix44_init_with_array(x_array);
}


matrix44
matrix44_init_with_array(const float array[])
{
  matrix44 return_mat;
  detail::internal_mat4 *internal_mat = reinterpret_cast<detail::internal_mat4*>(&return_mat);

  memcpy(internal_mat->data, array, sizeof(internal_mat->data));
  
  return return_mat;
}


matrix44
matrix44_init_with_array(const std::array<float, 16> &array)
{
  return matrix44_init_with_array(&array[0]);
}


matrix44
matrix44_init_lookat(const vector3 eye_position, const vector3 look_at_position, const vector3 up)
{
  const vector3 z_axis = vector3_normalize(vector3_subtract(look_at_position, eye_position));
  const vector3 x_axis = vector3_normalize(vector3_cross(z_axis, up));
  const vector3 y_axis = vector3_cross(x_axis, z_axis);

  const std::array<float, 16> array_mat =
  {
    vector3_get_x(x_axis),
    vector3_get_x(y_axis),
    -vector3_get_x(z_axis),
    0.f,

    vector3_get_y(x_axis),
    vector3_get_y(y_axis),
    -vector3_get_y(z_axis),
    0.f,

    vector3_get_z(x_axis),
    vector3_get_z(y_axis),
    -vector3_get_z(z_axis),
    0.f,

    -(vector3_dot(x_axis, eye_position)),
    -(vector3_dot(y_axis, eye_position)),
    +(vector3_dot(z_axis, eye_position)),
    1.f,
  };

  return matrix44_init_with_array(&array_mat[0]);
}


matrix44
matrix44_init_projection(const float width, const float height, const float near_plane, const float far_plane, const float fov)
{
  const float aspect_ratio = width / height;
  const float one_over_tan_half_fov = 1.f / caffmath::tan(fov * 0.5f);
  const float plane_diff = far_plane - near_plane;

  //matrix44 return_mat = matrix44_zero();
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

 return matrix44_init_with_array(&proj_mat[0]);
}


matrix44
matrix44_init_orthographic(const float width, const float height, const float depth)
{
  return matrix44_zero();
}


// Operations
matrix44
matrix44_add(const matrix44 &lhs, const matrix44 &rhs)
{
  const detail::internal_mat4 *left = reinterpret_cast<const detail::internal_mat4*>(&lhs);
  const detail::internal_mat4 *right = reinterpret_cast<const detail::internal_mat4*>(&rhs);

  matrix44 return_mat; 
  detail::internal_mat4 *internal_mat = reinterpret_cast<detail::internal_mat4*>(&return_mat);

  for(int i = 0; i < 16; ++i)
  {
    internal_mat->data[i] = left->data[0] + right->data[0];
  }

  return return_mat;
}


matrix44
matrix44_subtract(const matrix44 &lhs, const matrix44 &rhs)
{
  const detail::internal_mat4 *left = reinterpret_cast<const detail::internal_mat4*>(&lhs);
  const detail::internal_mat4 *right = reinterpret_cast<const detail::internal_mat4*>(&rhs);

  matrix44 return_mat; 
  detail::internal_mat4 *internal_mat = reinterpret_cast<detail::internal_mat4*>(&return_mat);

  for(int i = 0; i < 16; ++i)
  {
    internal_mat->data[i] = left->data[0] - right->data[0];
  }

  return return_mat;
}


matrix44
matrix44_scale(const float lhs, const matrix44 &rhs)
{
  const detail::internal_mat4 *right = reinterpret_cast<const detail::internal_mat4*>(&rhs);

  matrix44 return_mat; 
  detail::internal_mat4 *internal_mat = reinterpret_cast<detail::internal_mat4*>(&return_mat);

  for(int i = 0; i < 16; ++i)
  {
    internal_mat->data[i] = lhs * right->data[0];
  }

  return return_mat;
}


vector4
matrix44_multiply(const vector4 lhs, const matrix44 &rhs)
{
  const detail::internal_mat4 *right = reinterpret_cast<const detail::internal_mat4*>(&rhs);
  std::array<float, 4> vec_data;

  for(int i = 0; i < 16; i += 4)
  {
    const vector4 dot_vector = vector4_init(right->data[i + 0], right->data[i + 4], right->data[i + 8], right->data[i + 12]);

    vec_data.at(i / 4) = vector4_dot(lhs, dot_vector);
  }

  return vector4_init_with_array(vec_data);
}


matrix44
matrix44_multiply(const matrix44 &lhs, const matrix44 &rhs)
{
  const detail::internal_mat4 *left = reinterpret_cast<const detail::internal_mat4*>(&lhs);
  const detail::internal_mat4 *right = reinterpret_cast<const detail::internal_mat4*>(&rhs);

  matrix44 return_mat; 
  detail::internal_mat4 *internal_mat = reinterpret_cast<detail::internal_mat4*>(&return_mat);

  for(uint32_t i = 0; i < 16; ++i)
  {
    //[0,1,2,3] x [0,4,8,12]
    const uint32_t row = i / 4;
    const uint32_t col = i % 4;

    const vector4 left_vec = vector4_init(left->data[row + 0], left->data[row + 1], left->data[row + 2], left->data[row + 3]);
    const vector4 right_vec = vector4_init(right->data[col + 0], right->data[col + 4], right->data[col + 8], right->data[col + 12]);

    internal_mat->data[i] = vector4_dot(left_vec, right_vec);
  }

  return return_mat;
}


matrix44
matrix44_translate(const matrix44 &mat, const vector3 move)
{
  matrix44 copy(mat);

  detail::internal_mat4 *translate_me = reinterpret_cast<detail::internal_mat4*>(&copy);

  translate_me->data[12] += vector3_get_x(move);
  translate_me->data[13] += vector3_get_y(move);
  translate_me->data[14] += vector3_get_z(move);

  return copy;
}


matrix44
matrix44_rotate(const matrix44 &a, const vector3 euler)
{
  return matrix44();
}


float
matrix44_get(const matrix44 &mat, const uint32_t row, const uint32_t col)
{
  assert(row < 4 && col < 4);

  const uint32_t i = (row * 4) + col;
  return matrix44_get(mat, i);
}


float
matrix44_get(const matrix44 &mat, const uint32_t index)
{
  assert(index < 16);
  
  const detail::internal_mat4 *internal_mat = reinterpret_cast<const detail::internal_mat4*>(&mat);
  return internal_mat->data[index];
}


void
matrix44_set(matrix44 &mat, const uint32_t row, const uint32_t col, const float set)
{
  assert(row < 4 && col < 4);

  detail::internal_mat4 *internal_mat = reinterpret_cast<detail::internal_mat4*>(&mat);
  internal_mat->data[(row * 4) + col] = set;
}




} // namespace
} // namespace


#endif // include guard