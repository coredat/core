#include "physics_tick.hpp"

#include <transformations/entity/entity_transform.hpp>

#include <transformations/physics/bullet/bullet_math_extensions.hpp>
#include <transformations/physics/bullet/trigger_collisions.hpp>
#include <transformations/physics/collision_point.hpp>

#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include <BulletCollision/CollisionDispatch/btCollisionObject.h>
#include <utilities/bits.hpp>
#include <utilities/logging.hpp>

// Don't want core here! - Callback???
#include <core/common/collision.hpp>
#include <core/transform/transform.hpp>
#include <core/entity/detail/entity_id.hpp>
#include <core/entity/entity_ref.hpp>
#include <core/entity/entity_components.hpp>

#include <data/world_data.hpp>
#include <data/world/rigidbody_data.hpp>
#include <data/world/collision_data.hpp>
#include <data/graph/graph.hpp>
#include <utilities/id.hpp>




namespace {


// This state needs to go because its accross worlds
Core::Collision_callback callback_hack;

} // anon ns


namespace Engine {
namespace Physics_tick {


void
initialize(std::shared_ptr<Data::World> world)
{
  world->physics_world.dynamics_world->getBroadphase()->getOverlappingPairCache()->setInternalGhostPairCallback(new btGhostPairCallback());
  
  world->physics_world.dynamics_world->setInternalTickCallback(Physics_transform::trigger_callback,
                                                lib::id::id_to_ptr(world->world_instance_id),
                                                true);
}


void
collision_callback(Core::Collision_callback callback)
{
  callback_hack = callback;
};


void
think(std::shared_ptr<Data::World> world, const float dt, Tick_information *out_tick_info)
{
  
  /*
    Update the physics world
  */
    {
      world->physics_world.dynamics_world->stepSimulation(1 / 60.f, 500);
      world->physics_world.dynamics_world->debugDrawWorld();
    }

  
  /*
    Check collisions
  */
  {
  
  }


  /*
    Dispatch Collisions
    --
    Don't like the use of Core:: here.
    Where possible we should dump the information into an array, and let core apply
    the transformation to the data to generate Core::Collisions.
    
    Also don't like the callback here. World think should check if any collisions then fire its own callback.
  */
  {
    Data::Collision_data *collision_data(world->collision_data);
//    Data::Entity_data *entity_data(world->entity);
    
    Data::data_lock(collision_data);
//    Data::data_lock(entity_data);
  
    const uint32_t number_of_collisions(Data::collision_get_size(collision_data));
    
    uint32_t curr_contact(0);
    Core::Contact contacts[Core::Collision_detail::get_max_contacts()];
    
    for(uint32_t i = 0; i < number_of_collisions; ++i)
    {
      const uint64_t this_pair = collision_data->field_entity_pair[i];
      
      const Core_detail::Entity_id entity_a_id = Core_detail::entity_id_from_uint(lib::bits::lower32(this_pair));
      const Core::Entity_ref entity_a(entity_a_id);
      
      const Core_detail::Entity_id entity_b_id = Core_detail::entity_id_from_uint(lib::bits::upper32(this_pair));
      const Core::Entity_ref entity_b(entity_b_id);
      
      if(!entity_a || !entity_b)
      {
        LOG_WARNING_ONCE("We have an invalid entity in the collision.")
        continue;
      }
      
      // Add contact
      if(curr_contact < Core::Collision_detail::get_max_contacts())
      {
        Physics_transform::Collision_point collision_pt(Data::collision_get_collision_point_data(collision_data)[curr_contact + i]);
        
        contacts[curr_contact] = Core::Contact(entity_b,
                                                 collision_pt.point,
                                                 collision_pt.normal,
                                                 collision_pt.penitration);

      }
      else
      {
        LOG_WARNING_ONCE("We have more contacts than can report");
      }
      
      ++curr_contact;
      
      // If the next contact is different or we are at the end
      // Then dispatch the callback.
      
      uint64_t next_pair(0);
      if((i + 1) < number_of_collisions)
      {
        next_pair = collision_data->field_entity_pair[i + 1];
      }
      
      if(this_pair != next_pair)
      {
        // Dispatch Collision
        if(curr_contact > 0)
        {
          Core::Collision collision(entity_a, contacts, curr_contact);
          
          uintptr_t user_data = 0;
          Core::on_collision_callback_fn collision_callback = nullptr;
          
          Data::Graph::node_get_collision_callback(
            world->scene_graph,
            lib::bits::lower32(this_pair),
            &user_data,
            (uintptr_t*)&collision_callback
          );
          
//          Entity_detail::Callback_collision callback;



//          collision_callback = nullptr;
//          Data::Graph::node_get_
          
//          Entity_detail::Callback_collision collision_callback;
//          Data::entity_get_collision_callback(entity_data, lib::bits_lower(this_pair), &collision_callback);
          
          #ifdef CORE_COLLISION_DEBUG_TEXT
          printf("Collision: %s -> %s \n", entity_a.get_name(), entity_b.get_name());
          #endif
          
          if(collision_callback)
          {
            collision_callback(user_data, entity_a, collision);
          }
          
          curr_contact = 0;
        }
      }
    }
    Data::collision_clear(collision_data);
    Data::data_unlock(collision_data);
  }
}


void
de_initialize()
{
}


} // ns
} // ns
