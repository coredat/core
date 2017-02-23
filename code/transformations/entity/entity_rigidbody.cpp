#include <transformations/entity/entity_rigidbody.hpp>
#include <transformations/entity/entity_transform.hpp>
#include <transformations/physics/bullet/core_rb_to_bullet_rb.hpp>
#include <transformations/physics/bullet/bullet_math_extensions.hpp>
#include <transformations/physics/bullet/apply_force.hpp>
#include <core/physics/collider.hpp>
#include <core/physics/box_collider.hpp>
#include <core/physics/box_collider_utils.hpp>
#include <core/physics/collider_utils.hpp>
#include <core/physics/rigidbody.hpp>
#include <core/transform/transform.hpp>
#include <data/context_data.hpp>
#include <data/world_data.hpp>
#include <data/graph/graph.hpp>
#include <data/world/rigidbody_data.hpp>
#include <data/world/trigger_data.hpp>
#include <common/data_types.hpp>
#include <utilities/logging.hpp>
#include <utilities/bits.hpp>
#include <assert.h>


namespace {
  
  
// Errors
constexpr const char* err_no_force_cant_find_rb() {
  return "Failed to apply force can't find rigidbody";
}

constexpr const char* err_entity_has_rb_and_trigger() {
  return "Entity is both a trigger and rigidbody";
  }
  
  
} // anon ns


namespace Entity_detail {


void
set_rigidbody(const util::generic_id this_id,
              Core::Transform core_transform,
              const math::aabb &aabb,
              const Core::Rigidbody &rigidbody,
              Data::Graph::Graph_data *entity_data,
              Data::Trigger_data *trigger_data,
              Data::Rigidbody_data *rb_data,
              Bullet_data::World *physics_world)
{
  // Param check
  assert(entity_data);
  assert(trigger_data);
  assert(rb_data);
  assert(physics_world);
  
  btDynamicsWorld *phy_world = physics_world->dynamics_world;
  assert(phy_world);
  
  // Check and add component flag
  {
    if(entity_data)
    {
      uint32_t components(0);
      Data::Graph::components_get(entity_data, this_id, &components);
      
      if(Common::Data_type::is_collidable(components))
      {
        // Remove old rigidbody / trigger
        if(components & Common::Data_type::trigger)
        {
          Data::data_lock(trigger_data);
          
          Bullet_data::Trigger trigger;
          Data::trigger_get_trigger(trigger_data, this_id, &trigger);
          Bullet_data::remove_and_clear(&trigger, phy_world);
          
          Data::trigger_remove(trigger_data, this_id);
          
          Data::data_unlock(trigger_data);
        }
        else if(components & Common::Data_type::rigidbody)
        {
          Data::data_lock(rb_data);
          
          Bullet_data::Rigidbody rigidbody;
          Data::rigidbody_get_rigidbody(rb_data, this_id, &rigidbody);
          Bullet_data::remove_and_clear(&rigidbody, phy_world);
          Data::rigidbody_remove(rb_data, this_id);
          
          Data::data_unlock(rb_data);
        }
      }

      // Reset flag as it might change between one or the other.
      {
        components |= rigidbody.is_trigger() ? Common::Data_type::trigger : Common::Data_type::rigidbody;
        Data::Graph::components_set(entity_data, this_id, components);
      }
    }
  }

  // Common to trigger and rigidbody.
//  const Core::Transform core_transform(get_core_transform(this_id, entity_data, transform_data));

  // Add rigidbody
  if(!rigidbody.is_trigger())
  {
    assert(rb_data);
    
    if(rb_data)
    {
      // Set rb masking
      Data::data_lock(rb_data);
      
      Data::rigidbody_push(rb_data, this_id);
      
      const uint64_t mask = util::bits_pack(rigidbody.get_rb_id(), rigidbody.get_rb_mask());
      
      Data::rigidbody_set_collision_id(rb_data, this_id, &mask);
      
      Bullet_data::Rigidbody rb_details;
      Physics_transform::create_rigidbody_from_core_rb(
        &core_transform,
        aabb,
        &rigidbody,
        &rb_details,
        phy_world,
        this_id
      );

      Data::rigidbody_set_rigidbody(rb_data, this_id, &rb_details);
      
      Data::data_unlock(rb_data);
    }
  }
  
  // Add trigger
  else
  {
    assert(trigger_data);
    
    if(trigger_data)
    {
      Data::data_lock(trigger_data);
      Data::trigger_push(trigger_data, this_id);
      
      Bullet_data::Trigger trigger_details;
      Physics_transform::create_trigger_from_core_rb(
        &core_transform,
        aabb,
        &rigidbody,
        &trigger_details,
        phy_world,
        this_id
      );
      
      Data::trigger_set_trigger(trigger_data, this_id, &trigger_details);
      
      Data::data_unlock(trigger_data);
    }
  }
}


Core::Rigidbody
get_rigidbody(const util::generic_id this_id,
              const Data::Graph::Graph_data *entity,
              const Data::Rigidbody_data *rb_data,
              const Data::Trigger_data *trigger_data)
{
  // Param check
  assert(this_id);
  assert(entity);
  assert(rb_data);
  assert(trigger_data);
  
  // Get component list
  uint32_t components(0);
  Data::Graph::components_get(entity, this_id, &components);
  
  // Get local scale
  math::vec3 entity_scale = math::vec3_one();
  {
    math::transform transform;
    Data::Graph::transform_get(entity, this_id, &transform);
    
    entity_scale = transform.scale;
  }
  
  Core::Rigidbody core_rb;
  
  if(components & Common::Data_type::rigidbody)
  {
    Data::data_lock(rb_data);
    
    Bullet_data::Rigidbody rigidbody;
    Data::rigidbody_get_rigidbody(rb_data, this_id, &rigidbody);
    Physics_transform::create_core_rb_from_rigidbody(&core_rb, entity_scale, &rigidbody);
    
    Data::data_unlock(rb_data);
  }
  
  else if(components & Common::Data_type::trigger)
  {
    Data::data_lock(trigger_data);
    
    Bullet_data::Trigger trigger;
    Data::trigger_get_trigger(trigger_data, this_id, &trigger);
    Physics_transform::create_core_rb_from_trigger(&core_rb, entity_scale, &trigger);
    
    Data::data_unlock(trigger_data);
  }

  return core_rb;
}


void
set_phy_transform(const util::generic_id this_id,
                  const Core::Transform *transform,
                  Data::Graph::Graph_data *entity_data,
                  Data::Rigidbody_data *rb_data,
                  Bullet_data::World *phy_world,
                  Data::Trigger_data *trigger_data)
{
  // Param Check
  assert(this_id);
  assert(transform);
  assert(entity_data);
  assert(rb_data);
  assert(trigger_data);
  assert(phy_world);
  
  // Check if trigger or rigidbody
  bool is_trigger = false;
  bool is_rigidbody = false;
  {
    uint32_t components(0);
    Data::Graph::components_get(entity_data, this_id, &components);
    
    is_trigger   = (components & Common::Data_type::trigger);
    is_rigidbody = (components & Common::Data_type::rigidbody);
  }
  
  if(is_trigger == is_rigidbody)
  {
    assert(false);
    LOG_FATAL(err_entity_has_rb_and_trigger());
    return;
  }
  
  // New transform.
  const btTransform trans(math::transform_to_bt(*transform));
  
  if(is_rigidbody)
  {
    Data::data_lock(rb_data);
  
    Bullet_data::Rigidbody rigidbody;
    Data::rigidbody_get_rigidbody(rb_data, this_id, &rigidbody);
    
    Physics_transform::update_rigidbody_transform(&rigidbody,
                                                  phy_world,
                                                  &trans,
                                                  math::vec3_to_bt(transform->get_scale()));
    
    Data::data_unlock(rb_data);
  }
  else if(is_trigger)
  {
    Data::data_lock(trigger_data);
  
    Bullet_data::Trigger trigger;
    Data::trigger_get_trigger(trigger_data, this_id, &trigger);
    
    Physics_transform::update_trigger_transform(&trigger, &trans);
    
    Data::data_unlock(trigger_data);
  }
}


void
apply_force(const util::generic_id this_id,
            const Data::Rigidbody_data *rb_data,
            const math::vec3 direction,
            const float power)
{
  // Param check
  assert(this_id);
  assert(rb_data);
  assert(math::vec3_length(direction) > 0.f);
  
  Data::data_lock(rb_data);
  {
    Bullet_data::Rigidbody rigidbody;
    Data::rigidbody_get_rigidbody(rb_data, this_id, &rigidbody);
    
    if(rigidbody.rigidbody_ptr)
    {
      btRigidBody *bt_rb = reinterpret_cast<btRigidBody*>(rigidbody.rigidbody_ptr);
      
      Physics_transform::apply_world_force(bt_rb,
                                           math::vec3_to_bt(direction),
                                           btScalar(power));
    }
    else
    {
      LOG_ERROR(err_no_force_cant_find_rb());
    }
  }
  Data::data_unlock(rb_data);
}


} // ns