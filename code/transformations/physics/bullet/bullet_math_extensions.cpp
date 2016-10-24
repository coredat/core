#include <transformations/physics/bullet/bullet_math_extensions.hpp>
#include <LinearMath/btVector3.h>
#include <LinearMath/btTransform.h>
#include <core/transform/transform.hpp>
#include <math/quat/quat.hpp>
#include <math/vec/vec3.hpp>


namespace math {


math::vec3
vec3_from_bt(const btVector3 &bt_vec)
{
  return math::vec3_init(bt_vec.x(), bt_vec.y(), bt_vec.z());
}


btVector3
vec3_to_bt(const math::vec3 vec)
{
  return btVector3(math::get_x(vec), math::get_y(vec), math::get_z(vec));
}


btTransform
transform_to_bt(const Core::Transform &core_trans)
{
  btTransform trans;
  trans.setIdentity();
  
  // Rotation
  {
    const math::mat3 rot_mat    = math::quat_get_rotation_matrix(core_trans.get_rotation());
    const math::mat3 rot_mat_tr = math::mat3_get_transpose(rot_mat);
    
    const math::quat bt_rot = math::quat_init_with_mat3(rot_mat_tr);
    
    trans.setRotation(btQuaternion(math::get_x(bt_rot), math::get_y(bt_rot), math::get_z(bt_rot), math::get_w(bt_rot)));
  }
  
  // Position
  {
    trans.setOrigin(math::vec3_to_bt(core_trans.get_position()));
  }

  
  return trans;
}


Core::Transform
transform_from_bt(const btTransform &transform)
{
  Core::Transform trans;
  
  // Position is straight forward
  {
    trans.set_position(vec3_from_bt(transform.getOrigin()));
  }

  // Bullet has different axis
  // So we need to do a bit of work to convert rotatoins.
  {
    const btQuaternion &rotation = transform.getRotation();
    
    const math::quat rb_quat    = math::quat_init(rotation.x(), rotation.y(), rotation.z(), rotation.w());
    const math::mat3 rot_mat    = math::quat_get_rotation_matrix(rb_quat);
    const math::mat3 rot_mat_tr = math::mat3_get_transpose(rot_mat);
    const math::quat final_rot  = math::quat_init_with_mat3(rot_mat_tr);
    
    trans.set_rotation(final_rot);
  }
  
  return trans;
}


} // ns