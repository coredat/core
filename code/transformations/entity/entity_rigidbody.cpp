#include <transformations/entity/entity_rigidbody.hpp>
#include <transformations/entity/entity_transform.hpp>
#include <core/physics/collider.hpp>
#include <core/physics/box_collider.hpp>
#include <core/physics/collider_utils.hpp>
#include <core/physics/rigidbody.hpp>
#include <core/transform/transform.hpp>
#include <data/context_data.hpp>
#include <data/world_data.hpp>
#include <data/world/entity_data.hpp>
#include <data/world/transform_data.hpp>
#include <data/world/rigidbody_data.hpp>
#include <data/world/trigger_data.hpp>
#include <common/error_strings.hpp>
#include <common/data_types.hpp>
#include <utilities/logging.hpp>
#include <transformations/physics/bullet/core_rb_to_bullet_rb.hpp>
#include <transformations/physics/bullet/bullet_math_extensions.hpp>
#include <transformations/physics/bullet/apply_force.hpp>
#include <utilities/bits.hpp>
#include <assert.h>
#include <btBulletDynamicsCommon.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>


namespace Entity_detail {


void
set_collider(const util::generic_id this_id,
             Data::World *world,
             const Core::Collider &collider)
{
  if(!is_valid(this_id, world->entity))
  {
    LOG_ERROR(Error_string::entity_is_invalid());
    return;
  }
  
  auto rb_data = world->rigidbody_data;
  assert(rb_data);
  
  if(rb_data)
  {
    auto transform_data = world->transform;
    assert(transform_data);
  
    size_t index;
    if(Data::transform_exists(transform_data, this_id, &index))
    {
      //TODO: This can be async
      update_component(this_id, world->entity, Common::Data_type::rigidbody);
    
      switch(collider.get_type())
      {
        case(Core::Collider::Type::box):
        {
          const Core::Box_collider box_collider = Core::Collider_utis::cast_to_box_collider(collider);
          const math::vec3 box_scale = math::vec3_init(box_collider.get_x_half_extent() * 2.f,
                                                       box_collider.get_y_half_extent() * 2.f,
                                                       box_collider.get_z_half_extent() * 2.f);
          
          math::aabb collider_box = math::aabb_init(math::vec3_init(-0.5), math::vec3_init(+0.5));
          math::aabb_scale(collider_box, box_scale);
          
          // Get the current components and add physics
          {
            math::aabb entity_aabb;
            math::transform curr_transform;
            {
              Data::data_lock(world->transform);
              Data::transform_get_aabb(world->transform, this_id, &entity_aabb);
              Data::transform_get_transform(world->transform, this_id, &curr_transform);
              Data::data_unlock(world->transform);
            }
          
            // Set physics
            {
              Data::data_lock(rb_data);
              
              // Add the collider box
              if(Data::rigidbody_push(rb_data, this_id))
              {
//                Data::rigidbody_set_aabb_collider(rb_data, this_id, &collider_box);
              }
              
              Data::data_unlock(rb_data);
              
//              update_collider(this_id,
//                              world->entity,
//                              world->rigidbody_data,
//                              &curr_transform,
//                              &entity_aabb);
            }
          }
          break;
        }
        
        case(Core::Collider::Type::unknown):
        default:
          assert(false);
          LOG_ERROR(Error_string::unknown_type());
      }
    }
    else
    {
      LOG_ERROR(Error_string::entity_not_found());
      return;
    }
  }
  else
  {
    LOG_ERROR(Error_string::data_not_found());
  }
}


Core::Collider
get_collider(const util::generic_id this_id,
             Data::World *world)
{
  LOG_DEPRECATED_ONCE("get rb should do this now.");
  
  assert(false);
  LOG_ERROR(Error_string::no_implimentation());
  return Core::Collider();
}


void
set_rigidbody(const util::generic_id this_id,
              Data::World *world,
              const Core::Rigidbody &rigidbody)
{
  // Check and add component flag
  {
    Data::Entity_data *entity_data(world->entity);
    assert(entity_data);
    
    bool has_physics_component = false;
    
    if(entity_data)
    {
      Data::data_lock(entity_data);
      
      uint32_t components(0);
      Data::entity_get_components(entity_data, this_id, &components);
      
      if(Common::Data_type::is_collidable(components))
      {
        has_physics_component = true;
      }
      else
      {
        components |= rigidbody.is_trigger() ? Common::Data_type::trigger : Common::Data_type::rigidbody;
        Data::entity_set_components(entity_data, this_id, &components);
      }
      
      Data::data_unlock(entity_data);
    }
    
    if(has_physics_component)
    {
      assert(false);
      LOG_ERROR("Entity already has a physics component");
      return;
    }
  }

  // Common to trigger and rigidbody.
  const Core::Transform core_transform(get_core_transform(this_id, world->entity, world->transform));
  const Core::Collider core_collider(rigidbody.get_collider());
  const btTransform transform(math::transform_to_bt(core_transform));
  btCollisionShape *shape(Physics_transform::convert_core_collider_to_bullet_collider(&core_collider, &core_transform, this_id));

  // Add rigidbody
  if(!rigidbody.is_trigger())
  {
    auto rb_data = world->rigidbody_data;
    assert(rb_data);
    
    if(rb_data)
    {
      // Set rb masking
      Data::data_lock(rb_data);
      
      Data::rigidbody_push(rb_data, this_id);
      
      const uint64_t mask = util::bits_pack(rigidbody.get_rb_id(), rigidbody.get_rb_mask());
      
      Data::rigidbody_set_collision_id(rb_data, this_id, &mask);
      
      btRigidBody *bt_rb(Physics_transform::convert_core_rb_to_bullet_rb(&rigidbody, shape, &transform, this_id));
      
      world->dynamicsWorld->addRigidBody(bt_rb);
      
      const uintptr_t body_property(reinterpret_cast<uintptr_t>(bt_rb));
      Data::rigidbody_set_rigidbody(rb_data, this_id, &body_property);
      
      Data::data_unlock(rb_data);
    }
  }
  
  // Add trigger
  else
  {
    auto trigger_data = world->trigger_data;
    assert(trigger_data);
    
    if(trigger_data)
    {
      Data::data_lock(trigger_data);
      
      Data::trigger_push(trigger_data, this_id);
      
      btGhostObject *bt_ghost(Physics_transform::convert_core_rb_to_bullet_trigger(&rigidbody, shape, &transform));
      
      world->dynamicsWorld->addCollisionObject(bt_ghost);
      
      const uintptr_t trigger_property(reinterpret_cast<uintptr_t>(bt_ghost));
      Data::trigger_set_trigger(trigger_data, this_id, &trigger_property);
      
      Data::data_unlock(trigger_data);
    }
  }
}


Core::Rigidbody
get_rigidbody(const util::generic_id this_id)
{
  LOG_TODO_ONCE("Get rb doesn't build the rb");
  assert(false);

  return Core::Rigidbody();
}


void
set_phy_transform(const util::generic_id this_id,
                  const Core::Transform *transform,
                  Data::Entity_data *entity_data,
                  Data::Rigidbody_data *rb_data,
                     btDynamicsWorld *world,
                  Data::Trigger_data *trigger_data)
{
  // Param Check
  assert(this_id);
  assert(transform);
  assert(entity_data);
  assert(rb_data);
  assert(trigger_data);
  
  // Check if trigger or rigidbody
  bool is_trigger = false;
  bool is_rigidbody = false;
  {
    Data::data_lock(entity_data);
    
    uint32_t components(0);
    Data::entity_get_components(entity_data, this_id, &components);
    
    Data::data_unlock(entity_data);
    
    is_trigger   = (components & Common::Data_type::trigger);
    is_rigidbody = (components & Common::Data_type::rigidbody);
  }
  
  if(is_trigger == is_rigidbody)
  {
    LOG_ERROR("Can't set rigidbody/trigger transform");
    return;
  }
  
  if(is_trigger)
  {
    Data::data_lock(trigger_data);
  
    uintptr_t trigger(0);
    Data::trigger_get_trigger(trigger_data, this_id, &trigger);
    
    btPairCachingGhostObject *bt_trigger(reinterpret_cast<btPairCachingGhostObject*>(trigger));
    btTransform               trans(math::transform_to_bt(*transform));
    
    Physics_transform::update_trigger_transform(bt_trigger, &trans);
    
    Data::data_unlock(trigger_data);
  }
  
  if(is_rigidbody)
  {
    Data::data_lock(rb_data);
  
    uintptr_t rigidbody(0);
    Data::rigidbody_get_rigidbody(rb_data, this_id, &rigidbody);
    
    btRigidBody *bt_rigidbody(reinterpret_cast<btRigidBody*>(rigidbody));
    btTransform  trans(math::transform_to_bt(*transform));
    
    Physics_transform::update_rigidbody_transform(bt_rigidbody, world, &trans, math::vec3_to_bt(transform->get_scale()));
    
    Data::data_unlock(rb_data);
  }
}


void
update_collider(const util::generic_id this_id,
                Data::Entity_data *entity_data,
                Data::Rigidbody_data *phys_data,
                const math::transform *transform,
                const math::aabb *model_aabb,
                const bool inform_phys_engine)
{
//  // Check valid
//  if(!is_valid(this_id, entity_data, true)) {
//    assert(false); return;
//  }
//  
//  // Find the components
//  uint32_t components = 0;
//  {
//    assert(entity_data);
//    
//    Data::data_lock(entity_data);
//  
//    Data::entity_get_components(entity_data,
//                                this_id,
//                                &components);
//    
//    Data::data_unlock(entity_data);
//  }
//  
//  // If this is a physics object then update it.
//  {
//    assert(phys_data);
//  
//    // Update the physics stuff.
//    if(components & Common::Data_type::rigidbody)
//    {
//      Data::data_lock(phys_data);
//      
//      math::aabb collider_box;
//      Data::rigidbody_get_aabb_collider(phys_data, this_id, &collider_box);
//      
//      const math::vec3 collider_scale  = math::aabb_get_extents(collider_box);
//      const math::vec3 transform_scale = transform->scale;
//      const math::vec3 total_scale     = math::vec3_multiply(collider_scale, transform_scale);
//
//      // Order is important here! Scale then shift origin.
//      math::aabb_scale(collider_box, total_scale);
//      math::aabb_set_origin(collider_box, transform->position);
//      
//      Data::rigidbody_set_transform(phys_data, this_id, transform);
//      Data::rigidbody_set_transformed_aabb_collider(phys_data, this_id, &collider_box);
//      
//      if(inform_phys_engine)
//      {
//        uintptr_t body = 0;
//        Data::rigidbody_get_rigidbody(phys_data, this_id, &body);
//        
//        if(body)
//        {
//          q3Vec3 pos;
//          q3Vec3 axis;
//          f32 angle;
//          
//          math::transform_to_q3(transform, &angle, &axis, &pos);
//          
//          reinterpret_cast<q3Body*>(body)->SetTransform(pos, axis, angle);
//        }
//      }
//      
//      Data::data_unlock(phys_data);
//    }
//    else
//    {
////      assert(false);
//      LOG_TODO_ONCE("We should be able to check components before getting this far I think.");
//    }
//  } // if phys component
  
}


void
apply_force(const util::generic_id this_id,
            Data::Rigidbody_data *rb_data,
            const math::vec3 direction,
            const float power)
{
  assert(this_id);
  assert(math::vec3_length(direction) > 0.f);
  
  Data::data_lock(rb_data);
  {
    uintptr_t bt_data(0);
    Data::rigidbody_get_rigidbody(rb_data, this_id, &bt_data);
    
    if(bt_data)
    {
      btRigidBody *bt_rb = reinterpret_cast<btRigidBody*>(bt_data);
      
      Physics_transform::apply_world_force(bt_rb,
                                           math::vec3_to_bt(direction),
                                           btScalar(power));
    }
    else
    {
      LOG_ERROR("No RB Found to apply force to");
    }

  }
  Data::data_unlock(rb_data);
}


} // ns