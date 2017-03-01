#include "../physics.hpp"
#include "../contact.hpp"
#include "../config_physics.hpp"
#include "physics_data.hpp"
#include "utils/bullet_math_extensions.hpp"
#include <math/math.hpp>
#include <utilities/utilities.hpp>


// ------------------------------------------------------[ Physics Lifetime ]--


namespace Data {
namespace Physics {


void
world_set_gravity(Physics_data *phys, const math::vec3 gravity)
{
  LIB_ASSERT(false); // not impl
}


void
world_think(Physics_data *phys)
{
  // -- Param Check -- //
  LIB_ASSERT(phys);

  // Tick the world
  {
    phys->dynamics_world->stepSimulation(phys->time_step, 500);
  }
  
  // Draw the debug info
  #ifndef NDEBUG
  phys->dynamics_world->debugDrawWorld();
  #endif
  
  
//  /*
//    Dispatch Collisions
//    --
//    Don't like the use of Core:: here.
//    Where possible we should dump the information into an array, and let core apply
//    the transformation to the data to generate Core::Collisions.
//    
//    Also don't like the callback here. World think should check if any collisions then fire its own callback.
//  */
//  {
//    Data::Collision_data *collision_data(world->collision_data);
////    Data::Entity_data *entity_data(world->entity);
//    
//    Data::data_lock(collision_data);
////    Data::data_lock(entity_data);
//  
//    const uint32_t number_of_collisions(Data::collision_get_size(collision_data));
//    
//    uint32_t curr_contact(0);
//    Core::Contact contacts[Core::Collision_detail::get_max_contacts()];
//    
//    for(uint32_t i = 0; i < number_of_collisions; ++i)
//    {
//      const uint64_t this_pair = collision_data->field_entity_pair[i];
//      
//      const Core_detail::Entity_id entity_a_id = Core_detail::entity_id_from_uint(lib::bits::lower32(this_pair));
//      const Core::Entity_ref entity_a(entity_a_id);
//      
//      const Core_detail::Entity_id entity_b_id = Core_detail::entity_id_from_uint(lib::bits::upper32(this_pair));
//      const Core::Entity_ref entity_b(entity_b_id);
//      
//      if(!entity_a || !entity_b)
//      {
//        LOG_WARNING_ONCE("We have an invalid entity in the collision.")
//        continue;
//      }
//      
//      // Add contact
//      if(curr_contact < Core::Collision_detail::get_max_contacts())
//      {
//        Physics_transform::Collision_point collision_pt(Data::collision_get_collision_point_data(collision_data)[curr_contact + i]);
//        
//        contacts[curr_contact] = Core::Contact(entity_b,
//                                                 collision_pt.point,
//                                                 collision_pt.normal,
//                                                 collision_pt.penitration);
//
//      }
//      else
//      {
//        LOG_WARNING_ONCE("We have more contacts than can report");
//      }
//      
//      ++curr_contact;
//      
//      // If the next contact is different or we are at the end
//      // Then dispatch the callback.
//      
//      uint64_t next_pair(0);
//      if((i + 1) < number_of_collisions)
//      {
//        next_pair = collision_data->field_entity_pair[i + 1];
//      }
//      
//      if(this_pair != next_pair)
//      {
//        // Dispatch Collision
//        if(curr_contact > 0)
//        {
//          Core::Collision collision(entity_a, contacts, curr_contact);
//          
//          uintptr_t user_data = 0;
//          Core::on_collision_callback_fn collision_callback = nullptr;
//          
//          Data::Graph::node_get_collision_callback(
//            world->scene_graph,
//            lib::bits::lower32(this_pair),
//            &user_data,
//            (uintptr_t*)&collision_callback
//          );
//          
////          Entity_detail::Callback_collision callback;
//
//
//
////          collision_callback = nullptr;
////          Data::Graph::node_get_
//          
////          Entity_detail::Callback_collision collision_callback;
////          Data::entity_get_collision_callback(entity_data, lib::bits_lower(this_pair), &collision_callback);
//          
//          #ifdef CORE_COLLISION_DEBUG_TEXT
//          printf("Collision: %s -> %s \n", entity_a.get_name(), entity_b.get_name());
//          #endif
//          
//          if(collision_callback)
//          {
//            collision_callback(user_data, entity_a, collision);
//          }
//          
//          curr_contact = 0;
//        }
//      }
//    }
//    Data::collision_clear(collision_data);
//    Data::data_unlock(collision_data);  
}


void*
world_get_colliding_items(Physics_data *phys)
{
  return nullptr;
}


void
world_find_with_ray(
  Physics_data *phys,
  const math::vec3 origin,
  const math::vec3 scaled_direction,
  Contact *out_contacts,
  const uint32_t contacts_count,
  uint32_t *out_number_of_contacts)
{
  // -- Param Utils -- //
  LIB_ASSERT(phys);
  LIB_ASSERT(out_contacts);
  LIB_ASSERT(contacts_count > 0);
  LIB_ASSERT(out_number_of_contacts);
  
  // Convert to bullet
  const btVector3 start_pos(
    math::vec3_to_bt(origin)
  );
  
  const btVector3 end_pos(
    math::vec3_to_bt(
      math::vec3_add(
        origin,
        scaled_direction
      )
    )
  );
  
  btCollisionWorld::ClosestRayResultCallback ray_callback(
    start_pos, end_pos
  );
  
  phys->dynamics_world->rayTest(start_pos, end_pos, ray_callback);

  if(ray_callback.hasHit())
  {
    // Length from ray origin to the hit point.
    const float length(
      math::vec3_length(
        math::vec3_subtract(
          origin,
          math::vec3_from_bt(ray_callback.m_hitPointWorld)
        )
      )
    );
  
    const uintptr_t user_data(
      (uintptr_t)ray_callback.m_collisionObject->getCollisionShape()->getUserPointer()
    );
    
    out_contacts[0].contact_point  = math::vec3_from_bt(ray_callback.m_hitPointWorld);
    out_contacts[0].contact_normal = math::vec3_from_bt(ray_callback.m_hitNormalWorld);
    out_contacts[0].distance       = length;
    out_contacts[0].user_data      = user_data;
    
    *out_number_of_contacts = 1;
  }
}


} // ns
} // ns
