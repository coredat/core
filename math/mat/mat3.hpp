#ifndef MATRIX33_INCLUDED_D6DCE389_FA7C_4C48_A303_C0EDC9A90C8B
#define MATRIX33_INCLUDED_D6DCE389_FA7C_4C48_A303_C0EDC9A90C8B


/*
  Matrix 33
  Through out this library we consider a 3x3 matrix as a 
  rotation and scale matrix specifically.
*/


#include "mat_types.hpp"
#include "../vec/vec3.hpp"
#include <array>
#include <cstring>


namespace math {


// Constants
inline mat3                       mat3_id();
inline mat3                       mat3_zero();

// Init
inline mat3                       mat3_init(); // will return an id array
inline mat3                       mat3_init(const float x);
inline mat3                       mat3_init_with_array(const float arr[]);
inline mat3                       mat3_init_with_array(const std::array<float, 9> &array);

// Operations
inline mat3                       mat3_add(const mat3 &lhs, const mat3 &rhs);
inline mat3                       mat3_subtract(const mat3 &lhs, const mat3 &rhs);
inline mat3                       mat3_scale(const float scale, const mat3 &b);
inline mat3                       mat3_scale(const float x, const float y, const float z, const mat3 &b);
inline vec3                       mat3_multiply(const vec3 vec, const mat3 &b);
inline mat3                       mat3_multiply(const mat3 &lhs, const mat3 &rhs);
inline mat3                       mat3_rotate(const mat3 &lhs, const vec3 euler);

// Transform matrices into other forms
inline mat3                       mat3_get_transpose(const mat3 &a);
inline mat3                       mat3_get_inverse(const mat3 &a);
inline mat3                       mat3_get_scale(const mat3 &a, const vec3 scale);
inline void                       mat3_to_array(const mat3 &m, float *array);
inline std::array<float, 9>       mat3_to_array(const mat3 &m);

// Get/Set information
inline float                      mat3_get(const mat3 &mat, const uint32_t row, const uint32_t col);
inline float                      mat3_get(const mat3 &mat, const uint32_t i);
inline void                       mat3_set(mat3 &mat, const uint32_t row, const uint32_t col, const float set);
inline vec3                       mat3_get_scale(const mat3 &a);


// Impl

namespace detail
{
  struct internal_mat3
  {
    float data[9];
  };

}


mat3
mat3_id()
{
  std::array<float, 9> id_array = {
    1.f, 0.f, 0.f,
    0.f, 1.f, 0.f,
    0.f, 0.f, 1.f,
  };

  return mat3_init_with_array(&id_array[0]);
}


mat3
mat3_zero()
{
  std::array<float, 9> zero_array = {
    0.f, 0.f, 0.f,
    0.f, 0.f, 0.f,
    0.f, 0.f, 0.f,
  };

  return mat3_init_with_array(&zero_array[0]);  
}


mat3
mat3_init()
{
  return mat3_id();
}


mat3
mat3_init(const float x)
{
  std::array<float, 9> x_array = {
    x,x,x,
    x,x,x,
    x,x,x,
  };

  return mat3_init_with_array(x_array);
}


mat3
mat3_init_with_array(const float array[])
{
  mat3 return_mat;
  detail::internal_mat3 *internal_mat = reinterpret_cast<detail::internal_mat3*>(&return_mat);

  memcpy(internal_mat->data, array, sizeof(internal_mat->data));
  
  return return_mat;
}


mat3
mat3_init_with_array(const std::array<float, 9> &array)
{
  return mat3_init_with_array(&array[0]);
}


// Operations
mat3
mat3_add(const mat3 &lhs, const mat3 &rhs)
{
  const detail::internal_mat3 *left = reinterpret_cast<const detail::internal_mat3*>(&lhs);
  const detail::internal_mat3 *right = reinterpret_cast<const detail::internal_mat3*>(&rhs);

  mat3 return_mat; 
  detail::internal_mat3 *internal_mat = reinterpret_cast<detail::internal_mat3*>(&return_mat);

  for(int i = 0; i < 9; ++i)
  {
    internal_mat->data[i] = left->data[0] + right->data[0];
  }

  return return_mat;
}


mat3
mat3_subtract(const mat3 &lhs, const mat3 &rhs)
{
  const detail::internal_mat3 *left = reinterpret_cast<const detail::internal_mat3*>(&lhs);
  const detail::internal_mat3 *right = reinterpret_cast<const detail::internal_mat3*>(&rhs);

  mat3 return_mat; 
  detail::internal_mat3 *internal_mat = reinterpret_cast<detail::internal_mat3*>(&return_mat);

  for(int i = 0; i < 9; ++i)
  {
    internal_mat->data[i] = left->data[0] - right->data[0];
  }

  return return_mat;
}


mat3
mat3_scale(const float lhs, const mat3 &rhs)
{
  return mat3_scale(lhs, lhs, lhs, rhs);
}


mat3
mat3_scale(const float x, const float y, const float z, const mat3 &b)
{
  const detail::internal_mat3 *right = reinterpret_cast<const detail::internal_mat3*>(&b);

  mat3 return_mat; 
  detail::internal_mat3 *internal_mat = reinterpret_cast<detail::internal_mat3*>(&return_mat);

  internal_mat->data[0] = x * right->data[0];
  internal_mat->data[4] = y * right->data[0];
  internal_mat->data[8] = z * right->data[0];

  return return_mat;
}


vec3
mat3_multiply(const vec3 lhs, const mat3 &rhs)
{
  const detail::internal_mat3 *right = reinterpret_cast<const detail::internal_mat3*>(&rhs);
  std::array<float, 3> vec_data;

  for(int i = 0; i < 9; i += 4)
  {
    const vec3 dot_vec = vec3_init(right->data[i + 0], right->data[i + 3], right->data[i + 6]);

    vec_data.at(i / 3) = vec3_dot(lhs, dot_vec);
  }

  return vec3_init_with_array(vec_data);
}


mat3
mat3_multiply(const mat3 &lhs, const mat3 &rhs)
{
  const detail::internal_mat3 *left = reinterpret_cast<const detail::internal_mat3*>(&lhs);
  const detail::internal_mat3 *right = reinterpret_cast<const detail::internal_mat3*>(&rhs);

  mat3 return_mat; 
  detail::internal_mat3 *internal_mat = reinterpret_cast<detail::internal_mat3*>(&return_mat);

  for(uint32_t i = 0; i < 9; ++i)
  {
    //[0,1,2,3] x [0,4,8,12]
    const uint32_t row = i / 3;
    const uint32_t col = i % 3;

    const vec3 left_vec = vec3_init(left->data[row + 0], left->data[row + 1], left->data[row + 2]);
    const vec3 right_vec = vec3_init(right->data[col + 0], right->data[col + 3], right->data[col + 6]);

    internal_mat->data[i] = vec3_dot(left_vec, right_vec);
  }

  return return_mat;
}


mat3
mat3_rotate(const mat3 &a, const vec3 euler)
{
  return mat3();
}


float
mat3_get(const mat3 &mat, const uint32_t row, const uint32_t col)
{
  assert(row < 4 && col < 4);

  const uint32_t i = (row * 3) + col;
  return mat3_get(mat, i);
}


float
mat3_get(const mat3 &mat, const uint32_t index)
{
  assert(index < 9);
  
  const detail::internal_mat3 *internal_mat = reinterpret_cast<const detail::internal_mat3*>(&mat);
  return internal_mat->data[index];
}


void
mat3_set(mat3 &mat, const uint32_t row, const uint32_t col, const float set)
{
  assert(row < 4 && col < 4);

  detail::internal_mat3 *internal_mat = reinterpret_cast<detail::internal_mat3*>(&mat);
  internal_mat->data[(row * 4) + col] = set;
}




} // namespace


#endif // include guard