#ifndef ACTOR_INCLUDED_
#define ACTOR_INCLUDED_


#include <renderer/renderer.hpp>
#include <data/entity/entity_id.hpp>
#include <data/entity/entity_data.hpp>
#include <data/physics/physics.hpp>


namespace Actor {


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