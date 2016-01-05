#ifndef ACTOR_INCLUDED_
#define ACTOR_INCLUDED_


#include <renderer/renderer.hpp>
#include <data/entity/entity_id.hpp>
#include <data/entity/entity_data.hpp>
#include <data/physics/physics.hpp>


namespace Actor {


/*
  Actor Inputs
*/

struct Input_cmds
{
  int fwd   = 0;
  int right = 0;
  int rot = 0;
  int pitch = 0;
};


void
input(Input_cmds input, const float dt, const Entity_id id, Entity::Data *ents, const std::size_t size_of_data_entity_data, Physics::World *phy_world)
{
  auto local_controls = [&](const Entity_id ent)
  {
    const float delta_time = dt;
  
    const auto index = ents->find_index(ent);
    math::vec3 accum_movement = math::vec3_zero();
    const math::transform move_trans = ents->get_transform_data()[index];
    
    if(input.fwd > 0)
    {
      const math::vec3 move_fwd = math::vec3_init(0,0,-1);
      accum_movement = math::vec3_add(accum_movement, move_fwd);
    }

    if(input.fwd < 0)
    {
      const math::vec3 move_fwd = math::vec3_init(0,0,+1);
      accum_movement = math::vec3_add(accum_movement, move_fwd);
    }
    
    if(input.right > 0)
    {
      const math::vec3 move_fwd = math::vec3_init(-1,0,0);
      accum_movement = math::vec3_add(accum_movement, move_fwd);
    }
    
    if(input.right < 0)
    {
      const math::vec3 move_fwd = math::vec3_init(+1,0,0);
      accum_movement = math::vec3_add(accum_movement, move_fwd);
    }
    
    if(math::vec3_length(accum_movement) != 0)
    {
      const math::vec3 norm_movement            = math::vec3_normalize(accum_movement);
      const math::vec3 rotated_movement         = math::quat_rotate_point(move_trans.rotation, norm_movement);
      const math::vec3 corrected_rotation       = math::vec3_init(math::vec3_get_x(rotated_movement), 0.f, math::vec3_get_z(rotated_movement)); // We're not interested in y movement.
      const math::vec3 norm_corrected_rotation  = math::vec3_normalize(corrected_rotation);
      const math::vec3 scaled_movement          = math::vec3_scale(norm_corrected_rotation, delta_time);
      
      ents->get_transform_data()[index].position = math::vec3_add(move_trans.position, scaled_movement);
    }
          // Rotation
    {
      if(input.rot != 0)
      {
        const math::transform rot_trans = ents->get_transform_data()[index];
        const float rot_rad = static_cast<float>(input.rot) * delta_time;
        
        const math::quat rot = math::quat_init_with_axis_angle(0, 1, 0, rot_rad);
        ents->get_transform_data()[index].rotation = math::quat_multiply(rot_trans.rotation, rot);
      }
    }
    
    // Head
    {
      if(input.pitch != 0)
      {
        const math::transform rot_trans = ents->get_transform_data()[index];
        const float rot_rad = static_cast<float>(input.pitch) * delta_time;
        
        const math::vec3 head_axis = math::vec3_init(1,0,0);
        const math::vec3 adjusted_axis = math::quat_rotate_point(rot_trans.rotation, head_axis);
        
        const math::quat rot = math::quat_init_with_axis_angle(adjusted_axis, rot_rad);
        ents->get_transform_data()[index].rotation = math::quat_multiply(rot_trans.rotation, rot);
      }
    }
  };
  
  local_controls(id);
  
}


void
update(const Entity_id id, Entity::Data *ents, const std::size_t size_of_data_entity_data, Physics::World *phy_world)
{
  auto apply_gravity = [&](const Entity_id ent)
  {
    struct Kine_actor
    {
      bool is_grounded = false;
    } act;
  
    // Get transform of controller.
    const auto index = ents->find_index(ent);
    const math::transform curr_trans = ents->get_transform_data()[index];
    
    // Cast ray downwards
    btVector3 btFrom(math::vec3_get_x(curr_trans.position), math::vec3_get_y(curr_trans.position), math::vec3_get_z(curr_trans.position));
    btVector3 btTo(math::vec3_get_x(curr_trans.position), math::vec3_get_y(curr_trans.position) - 2, math::vec3_get_z(curr_trans.position));
    btCollisionWorld::ClosestRayResultCallback feet_test(btFrom, btTo);
    
    phy_world->dynamics_world.rayTest(btFrom, btTo, feet_test);

    Renderer::debug_line(btFrom, btTo, btVector3(1, 1, 0));
    
    // If not hit anything then go downwards.
    // Gravity
    if(!feet_test.hasHit())
    {
      const math::vec3 down = math::vec3_init(0, -0.01, 0);
      
      math::transform new_trans = curr_trans;
      new_trans.position = math::vec3_add(curr_trans.position, down);
      
      ents->get_transform_data()[index] = new_trans;
    }
    else
    {
      act.is_grounded = true;
    }
  };
  
  apply_gravity(id);
}



} // ns


#endif // inc guard