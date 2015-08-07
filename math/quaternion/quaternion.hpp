#ifndef QUATERNION_INCLUDED_ED2C1105_82A1_477F_B62C_5093DE20C971
#define QUATERNION_INCLUDED_ED2C1105_82A1_477F_B62C_5093DE20C971

/*
  Quaternion
  Used for rotation math.
*/


#include "quat_types.hpp"
#include "../general/general.hpp"
#include "../mat/mat4.hpp"
#include "../mat/mat3.hpp"
#include "../vec/vec3.hpp"


namespace math {


// Interface

inline quat             quat_init();
inline quat             quat_init(const float x, const float y, const float z, const float w);
inline quat             quat_init_with_axis_angle(const float x, const float y, const float z, const float theta_radians);
inline quat             quat_init_with_euler_angles(const float pitch_radians, const float yaw_radians, const float roll_radians);

inline quat             quat_conjugate(const quat to_conj);
inline quat             quat_multiply(const quat left, const quat &right);
inline quat             quat_normalize(const quat to_normalize);
inline float            quat_length(const quat to_length);
inline vec3             quat_rotate_point(const quat rotation, const vec3 point);

inline mat3             quat_get_rotation_matrix();
inline vec3             quat_get_axis();
inline vec3             quat_get_euler_angles_in_radians();

inline float            quat_get_x(const quat quat);
inline float            quat_get_y(const quat quat);
inline float            quat_get_z(const quat quat);
inline float            quat_get_w(const quat quat);
inline float            quat_get(const quat quat, const uint32_t i);


// Impl

namespace detail
{
  struct internal_quat
  {
    float x, y, z, w;
  };
}


quat
quat_init()
{
  return quat_init(0,0,0,1);
}


quat
quat_init(const float x, const float y, const float z, const float w)
{
  quat return_quat;
  detail::internal_quat *internal_quat = reinterpret_cast<detail::internal_quat*>(&return_quat);

  internal_quat->x = x;
  internal_quat->y = y;
  internal_quat->z = z;
  internal_quat->w = w;

  return return_quat;
}


quat
quat_init_with_axis_angle(const float x, const float y, const float z, const float theta_radians)
{
  // TODO: Normalize Axis.

  const float half_angle = 0.5f * theta_radians;
  const float sin_angle  = caffmath::sin(half_angle);

  const float qx = sin_angle * x;
  const float qy = sin_angle * y;
  const float qz = sin_angle * z;
  const float qw = caffmath::cos(half_angle);  

  return quat_init(qx, qy, qz, qw);
}


quat
quat_init_with_euler_angles(const float pitch_radians, const float yaw_radians, const float roll_radians)
{
  const float half_roll  = roll_radians * 0.5f;
  const float half_yaw   = yaw_radians * 0.5f;
  const float half_pitch = pitch_radians * 0.5f;

  const float cz = caffmath::cos(half_roll);
  const float sz = caffmath::sin(half_roll);
  const float cy = caffmath::cos(half_yaw);
  const float sy = caffmath::sin(half_yaw);
  const float cx = caffmath::cos(half_pitch);
  const float sx = caffmath::sin(half_pitch);

  const float w = cz * cy * cx + sz * sy * sz;
  const float x = cz * cy * sx - sz * sy * cz;
  const float y = cz * sy * cx + sz * cy * sz;
  const float z = sz * cy * cx - cz * sy * sz;

  return quat_init(x, y, z, w);
}


quat
quat_conjugate(const quat to_conj)
{
  const detail::internal_quat *conj_quat = reinterpret_cast<const detail::internal_quat*>(&to_conj);
  return quat_init(-conj_quat->x, -conj_quat->y, -conj_quat->z, conj_quat->w);
}


quat
quat_multiply(const quat left, const quat &right)
{
  const detail::internal_quat *left_quat  = reinterpret_cast<const detail::internal_quat*>(&left);
  const detail::internal_quat *right_quat = reinterpret_cast<const detail::internal_quat*>(&right);

  const float w = (left_quat->w * right_quat->w) - (left_quat->x * right_quat->x) - (left_quat->y * right_quat->y) - (left_quat->z * right_quat->z);
  const float x = (left_quat->w * right_quat->x) + (left_quat->x * right_quat->w) + (left_quat->y * right_quat->z) - (left_quat->z * right_quat->y);
  const float y = (left_quat->w * right_quat->y) + (left_quat->y * right_quat->w) + (left_quat->z * right_quat->x) - (left_quat->x * right_quat->z);
  const float z = (left_quat->w * right_quat->z) + (left_quat->z * right_quat->w) + (left_quat->x * right_quat->y) - (left_quat->y * right_quat->x);

  return quat_init(x, y, z, w);
}


quat
quat_normalize(const quat to_normalize)
{
  const detail::internal_quat *internal_quat = reinterpret_cast<const detail::internal_quat*>(&to_normalize);

  const float length = quat_length(to_normalize);
  assert(length); // Can't have zero length.

  const float w = internal_quat->w / length;
  const float x = internal_quat->x / length;
  const float y = internal_quat->y / length;
  const float z = internal_quat->z / length;

  return quat_init(x,y,z,w);
}


float
quat_length(const quat to_length)
{
  const detail::internal_quat *length_quat = reinterpret_cast<const detail::internal_quat*>(&to_length);
  return caffmath::sqrt((length_quat->w * length_quat->w) + (length_quat->x * length_quat->x) + (length_quat->y * length_quat->y) + (length_quat->z * length_quat->z));
}


vec3
quat_rotate_point(const quat rotation, const vec3 point)
{
  const detail::internal_quat *rot_quat = reinterpret_cast<const detail::internal_quat*>(&rotation);

  const float x = rot_quat->x;
  const float y = rot_quat->y;
  const float z = rot_quat->z;
  const float w = rot_quat->w;

  const float p_x = w*w*vec3_get_x(point) + 2*y*w*vec3_get_z(point) - 2*z*w*vec3_get_y(point) + x*x* vec3_get_x(point) + 2*y*x*vec3_get_y(point) + 2*z*x*vec3_get_z(point) - z*z*vec3_get_x(point) - y*y*vec3_get_x(point);
  const float p_y = 2*x*y*vec3_get_x(point) + y*y*vec3_get_y(point) + 2*z*y*vec3_get_z(point) + 2*w*z* vec3_get_x(point) - z*z*vec3_get_y(point) + w*w*vec3_get_y(point) - 2*x*w*vec3_get_z(point) - x*x*vec3_get_y(point);
  const float p_z = 2*x*z*vec3_get_x(point) + 2*y*z*vec3_get_y(point) + z*z*vec3_get_z(point) - 2*w*y* vec3_get_x(point) - y*y*vec3_get_z(point) + 2*w*x*vec3_get_y(point) - x*x*vec3_get_z(point) + w*w*vec3_get_z(point);

  const vec3 rotated_point = vec3_init(p_x, p_y, p_z);

  return rotated_point;
}


// mat3
// quat_get_rotation_matrix()
// {

// }


// vec3
// quat_get_axis()
// {

// }


// vec3
// quat_get_euler_angles_in_radians()
// {

// }


float
quat_get_x(const quat quat)
{
  const detail::internal_quat *internal_quat = reinterpret_cast<const detail::internal_quat*>(&quat);
  return internal_quat->x;
}


float
quat_get_y(const quat quat)
{
  const detail::internal_quat *internal_quat = reinterpret_cast<const detail::internal_quat*>(&quat);
  return internal_quat->y;
}


float
quat_get_z(const quat quat)
{
  const detail::internal_quat *internal_quat = reinterpret_cast<const detail::internal_quat*>(&quat);
  return internal_quat->z;
}


float
quat_get_w(const quat quat)
{
  const detail::internal_quat *internal_quat = reinterpret_cast<const detail::internal_quat*>(&quat);
  return internal_quat->w;
}


float
quat_get(const quat quat, const uint32_t i)
{
  assert(i < 4);

  switch(i)
  {
    case(0):
      return quat_get_x(quat);
      break;
    case(1):
      return quat_get_y(quat);
      break;
    case(2):
      return quat_get_z(quat);
      break;
    case(3):
      return quat_get_w(quat);
      break; 
    default:
      assert(false);
  }
}


} // namespace


#endif // include guard