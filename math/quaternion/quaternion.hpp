#ifndef QUATERNION_INCLUDED_ED2C1105_82A1_477F_B62C_5093DE20C971
#define QUATERNION_INCLUDED_ED2C1105_82A1_477F_B62C_5093DE20C971


// ** NOT COMPLETE ** //


#include "quaternion_types.hpp"
#include "../general/general.hpp"
#include "../matrix/matrix44.hpp"
#include "../matrix/matrix33.hpp"
#include "../vector/vector3.hpp"


namespace caffmath = ::caffeine::math;

namespace caffeine {
namespace math {


// Interface

inline quaternion       quaternion_init();
inline quaternion       quaternion_init(const float x, const float y, const float z, const float w);
inline quaternion       quaternion_init_with_axis_angle(const float x, const float y, const float z, const float theta_rad);
inline quaternion       quaternion_init_with_euler_angles(const float pitch_rad, const float yaw_rad, const float roll_rad);

inline quaternion       quaternion_conjugate(const quaternion to_conj);
inline quaternion       quaternion_multiply(const quaternion left, const quaternion &right);
inline quaternion       quaternion_normalize(const quaternion to_conj);
inline quaternion       quaternion_length(const quaternion to_conj);
inline vector3          quaternion_rotate_point(const quaternion rotation, const vector3 point);

inline matrix33         quaternion_get_rotation_matrix();
inline vector3          quaternion_get_axis();
inline vector3          quaternion_get_euler_angles();


// Impl

namespace detail
{
  struct internal_quat
  {
    float x, y, z, w;
  };
}


quaternion
quaternion_init()
{
  return quaternion_init(0,0,0,1);
}


quaternion
quaternion_init(const float x, const float y, const float z, const float w)
{
  quaternion return_quat;
  detail::internal_quat *internal_quat = reinterpret_cast<detail::internal_quat*>(&return_quat);

  internal_quat->x = x;
  internal_quat->y = y;
  internal_quat->z = z;
  internal_quat->w = w;

  return return_quat;
}


quaternion
quaternion_init_with_axis_angle(const float x, const float y, const float z, const float theta_rad)
{
  // TODO: Normalize Axis.

  const float half_angle = 0.5f * theta_radians;
  const float sin_angle  = caffmath::sin(half_angle);

  const float x = sin_angle * axis.x;
  const float y = sin_angle * axis.y;
  const float z = sin_angle * axis.z;
  const float w = caffmath::cos(half_angle);  

  return quaternion_init(x, y, z, w);
}


quaternion
quaternion_init_with_euler_angles(const float pitch_rad, const float yaw_rad, const float roll_rad)
{
  const float half_roll  = roll_rad * 0.5f;
  const float half_yaw   = yaw_rad * 0.5f;
  const float half_pitch = pitch_rad * 0.5f;

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

  return quaternion_init(x, y, z, w);
}



} // namespace
} // namespace


#endif // include guard