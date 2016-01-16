#ifndef ACTOR_INCLUDED_0763D65A_3038_43D9_837E_88628EE7D27F
#define ACTOR_INCLUDED_0763D65A_3038_43D9_837E_88628EE7D27F


#include <renderer/renderer.hpp>
#include <systems/entity/entity_id.hpp>
#include <systems/physics/physics.hpp>
#include <data/entity_pool.hpp>


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


inline void
input(Input_cmds input,
      const float dt,
      const Entity::Entity_id id,
      Data::Entity_pool *ents,
      const std::size_t size_of_data_entity_data,
      Physics::World *phy_world)
{
  auto local_controls = [&](const Entity::Entity_id ent)
  {
    const float delta_time = dt;
    
    std::size_t index;
    Entity::find_index_linearly(&index, ent, ents->entity_id, size_of_data_entity_data);
  
    math::vec3 accum_movement = math::vec3_zero();
    const math::transform move_trans = ents->transform[index];
    
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
      
      ents->transform[index].position = math::vec3_add(move_trans.position, scaled_movement);
    }
          // Rotation
    {
      if(input.rot != 0)
      {
        const math::transform rot_trans = ents->transform[index];
        const float rot_rad = static_cast<float>(input.rot) * delta_time;
        
        const math::quat rot = math::quat_init_with_axis_angle(0, 1, 0, rot_rad);
        ents->transform[index].rotation = math::quat_multiply(rot_trans.rotation, rot);
      }
    }
    
    // Head
    {
      if(input.pitch != 0)
      {
        const math::transform rot_trans = ents->transform[index];
        const float rot_rad = static_cast<float>(input.pitch) * delta_time;
        
        const math::vec3 head_axis = math::vec3_init(1,0,0);
        const math::vec3 adjusted_axis = math::quat_rotate_point(rot_trans.rotation, head_axis);
        
        const math::quat rot = math::quat_init_with_axis_angle(adjusted_axis, rot_rad);
        ents->transform[index].rotation = math::quat_multiply(rot_trans.rotation, rot);
      }
    }
  };
  
  local_controls(id);
}


inline void
update(const Entity::Entity_id id, Data::Entity_pool *ents, const std::size_t size_of_data_entity_data, Physics::World *phy_world)
{
  auto apply_gravity = [&](const Entity::Entity_id ent)
  {
    struct Kine_actor
    {
      bool is_grounded = false;
    } act;
  
    // Get transform of controller.
    std::size_t index;
    Entity::find_index_linearly(&index, ent, ents->entity_id, size_of_data_entity_data);

    const math::transform curr_trans = ents->transform[index];
    
    // Cast ray downwards
    const btVector3 btFrom(math::vec3_get_x(curr_trans.position), math::vec3_get_y(curr_trans.position), math::vec3_get_z(curr_trans.position));
    const btVector3 btTo(math::vec3_get_x(curr_trans.position), math::vec3_get_y(curr_trans.position) - 2, math::vec3_get_z(curr_trans.position));
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
      
      ents->transform[index] = new_trans;
    }
    else
    {
      const auto norm = feet_test.m_hitPointWorld;

      math::transform new_trans = curr_trans;
      math::vec3 pos = math::vec3_init(math::vec3_get_x(curr_trans.position), norm.y() + 2, math::vec3_get_z(curr_trans.position));
      new_trans.position = pos;
      
      ents->transform[index] = new_trans;
      
      act.is_grounded = true;
    }
  };
  
  apply_gravity(id);
}



} // ns


#endif // inc guard