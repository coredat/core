#include "convert.hpp"
#include "../../config_collider.hpp"
#include "bullet_math_extensions.hpp"
#include <btBulletDynamicsCommon.h>
#include <math/math.hpp>
#include <utilities/optimizations.hpp>
#include <utilities/assert.hpp>


namespace Bullet_detail {


btCollisionShape*
generate_collision_shape(
  const Data::Physics::Collider_config *collider,
  const uintptr_t user_data,
  const math::vec3 scale)
{
  switch(collider->type)
  {
    /*
      We create the colliders to default sizes, and scale using localscaling.
    */
    case(Data::Physics::Collider_type::box):
    {
      const btVector3 bt_half_extents(0.5f, 0.5f, 0.5f);
      
      btBoxShape *bt_collider = new btBoxShape(bt_half_extents);
   
//      const Core::Box_collider core_box(
//        Core::Collider_utis::cast_to_box_collider(collider)
//      );
      
//      const math::vec3 half_extents = Core::Box_collider_utils::get_full_extents(core_box);
//      const math::vec3 scaled_extents(math::vec3_multiply(half_extents, scale));
//      const btVector3 scale_vector = math::vec3_to_bt(scaled_extents);

      const btVector3 scale_vector = math::vec3_to_bt(scale);
      
      bt_collider->setLocalScaling(scale_vector);
      bt_collider->setUserPointer((void*)user_data);
    
      return bt_collider;
    
      break;
    }
    
    default:
      LIB_ASSERT(false);
  };
  
  UNREACHABLE;
  
  return nullptr;
}


Data::Physics::Collider_config
generate_collider(
  const btCollisionShape *shape,
  const math::vec3 scale)
{
  const math::vec3 local_scale = math::vec3_from_bt(shape->getLocalScaling());
  const math::vec3 extents     = math::vec3_divide(scale, local_scale);
  
  switch(shape->getShapeType())
  {
    case(BOX_SHAPE_PROXYTYPE):
    {
      return Data::Physics::Collider_config
      {
        Data::Physics::Collider_type::box,
        {
          math::get_x(extents),
          math::get_y(extents),
          math::get_z(extents),
          0.f
        }
      };
    }
    
    default:
      LIB_ASSERT(false);
  }
  
  UNREACHABLE;
  
  return Data::Physics::Collider_config{};
}


btTransform
generate_transform(
  const math::transform *trans,
  const math::aabb *aabb)
{
  const btTransform bt_trans = math::transform_to_bt(*trans);
  
  math::vec3 rotated_origin = math::quat_rotate_point(trans->rotation, math::aabb_get_origin(*aabb));
  
  const btVector3 origin = math::vec3_to_bt(rotated_origin);
  const btVector3 offset_pos = bt_trans.getOrigin() + origin;
  
  return btTransform(bt_trans.getRotation(), offset_pos);
  
//  const math::vec3 origin(math::aabb_get_origin(*aabb));
//  const math::vec3 offset(math::vec3_add(trans->position, origin));
//  
//  return math::transform_to_bt(
//    math::transform_init(offset, trans->scale, trans->rotation)
//  );
}


} // ns
