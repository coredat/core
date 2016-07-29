#include <transformations/physics/q3_math_extensions.hpp>
#include <3rdparty/qu3e/math/q3Vec3.h>
#include <3rdparty/qu3e/math/q3Quaternion.h>
#include <3rdparty/qu3e/math/q3Transform.h>
#include <assert.h>


namespace math {


q3Vec3
vec3_to_q3vec(const math::vec3 vec)
{
  return q3Vec3(math::get_x(vec),
                math::get_y(vec),
                math::get_z(vec));
}


q3Quaternion
quat_to_q3quat(const math::quat quat)
{
  return q3Quaternion(math::quat_get_x(quat),
                      math::quat_get_y(quat),
                      math::quat_get_z(quat),
                      math::quat_get_w(quat));
}


vec3
vec3_from_q3vec(const q3Vec3 vec)
{
  return math::vec3_init(vec.x, vec.y, vec.z);
}


math::transform
transform_init_from_q3(const q3Transform transform)
{
  math::vec3 pos = vec3_from_q3vec(transform.position);
  
  // This is transpose.
  const float arr_mat[] = {
    transform.rotation.ex.x, transform.rotation.ex.y, transform.rotation.ex.z,
    transform.rotation.ey.x, transform.rotation.ey.y, transform.rotation.ey.z,
    transform.rotation.ez.x, transform.rotation.ez.y, transform.rotation.ez.z,
  };
  
  const math::mat3 q3_rot_mat     = math::mat3_init_with_array(arr_mat);
//  const math::mat3 q3_rot_mat_tr  = math::mat3_get_transpose(q3_rot_mat);
  const math::quat final_rot      = math::quat_init_with_mat3(q3_rot_mat);

  return math::transform_init(pos, math::vec3_one(), final_rot);
}


void
transform_to_q3(const math::transform *math_transform,
                r32 *out_angle,
                q3Vec3 *out_axis,
                q3Vec3 *out_position)
{
  assert(math_transform);

  math::quat friendly_rot;
  {
    const math::mat3 rot_mat    = math::quat_get_rotation_matrix(math_transform->rotation);
    const math::mat3 rot_mat_tr = math::mat3_get_transpose(rot_mat);
    
    friendly_rot = math::quat_init_with_mat3(rot_mat_tr);
  }

  const q3Quaternion quat = quat_to_q3quat(friendly_rot);

  r32 angle = 0;
  q3Vec3 axis;

  quat.ToAxisAngle(&axis, &angle);
  
  if(out_angle)     { *out_angle = angle;                                      }
  if(out_axis)      { *out_axis = axis;                                        }
  if(out_position)  { *out_position = vec3_to_q3vec(math_transform->position); }
}



} // ns