#include "../physics.hpp"
#include "../contact.hpp"
#include "../config_physics.hpp"
#include "physics_data.hpp"
#include "utils/bullet_math_extensions.hpp"
#include <math/math.hpp>
#include <utilities/utilities.hpp>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>


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
  
  // Collisions //
  
//  auto                   world_data(Core_detail::world_index_get_world_data(world_id));
//  Data::Collision_data  *collision_data(world_data->collision_data);
//  Data::Trigger_data    *trigger_data(world_data->trigger_data);
//  
//  if(!world_data || !collision_data || !trigger_data)
//  {
//    LOG_FATAL("Missing Data");
//    return;
//  }
  
  
//  /*
//    Trigger Collisions
//  */
//  {
//    const size_t trigger_count(phys->trigger_data.size());
//    
////    for(size_t t = 0; t < trigger_count; ++t)
//    for(auto &trig : phys->trigger_data)
//    {
//      uintptr_t ghost_trigger(trig.ghost_ptr);
//      
//      btPairCachingGhostObject *ghost_object(reinterpret_cast<btPairCachingGhostObject*>(ghost_trigger));
//      
//      if(!ghost_object)
//      {
//        LOG_ERROR_ONCE("Null trigger found.");
//        continue;
//      }
//    
//      struct callback : public btCollisionWorld::ContactResultCallback
//      {
//        virtual	btScalar
//        addSingleResult(btManifoldPoint &manifold,
//                        const btCollisionObjectWrapper* collision_shape_a,
//                        int partId0,
//                        int index0,
//                        const btCollisionObjectWrapper* collision_shape_b,
//                        int partId1,
//                        int index1)
//        {
//          // http://bulletphysics.org/Bullet/phpBB3/viewtopic.php?t=5982#p20990
//          constexpr btScalar unused_return = 1.f;
//        
//          const uintptr_t entity_id_a((uintptr_t)collision_shape_a->getCollisionShape()->getUserPointer());
//          const uintptr_t entity_id_b((uintptr_t)collision_shape_b->getCollisionShape()->getUserPointer());
//          
////          #ifdef EXTRA_DEBUG_INFO_TRIGGER_CALLBACK
////          Core::Entity_ref ref_a(Core_detail::entity_id_from_uint(entity_id_a));
////          Core::Entity_ref ref_b(Core_detail::entity_id_from_uint(entity_id_b));
////          #endif
//          
//          if(!entity_id_a || !entity_id_b)
//          {
//            LOG_WARNING_ONCE("Found a collision shape with no user-data");
//            return unused_return;
//          }
//          
//          const float penitration(manifold.getDistance());
//          
//          if(penitration < 0.f)
//          {
//            const uint32_t collision_id(Data::collision_push(collision_data));
//            const float direction = 1.f;
//
//            Physics_transform::Collision_point contact
//            {
//              penitration * direction,
//              math::vec3_from_bt(manifold.m_positionWorldOnA),
//              math::vec3_from_bt(manifold.m_normalWorldOnB)
//            };
//            
//            Data::collision_set_collision_point(collision_data, collision_id, &contact);
//            
//            const uint64_t entity_pair(lib::bits::pack3232(entity_id_a, entity_id_b));
//            Data::collision_set_entity_pair(collision_data, collision_id, &entity_pair);
//          }
//          
//          return unused_return;
//        }
//        
//        Data::Collision_data *collision_data;
//      };
//
//      callback cb;
//      cb.collision_data = collision_data;
//
//      dynamicsWorld->contactTest(ghost_object, cb);
//    
//    }
//  }
//  
//  /*
//    Trigger Collisions
//    --
//    For each trigger check overlapping pairs.
//  */
//  {
//    const size_t trigger_count(Data::trigger_get_size(trigger_data));
//
//    // Disabling this for the moment.
//    for(size_t t = trigger_count; t < trigger_count; ++t)
////    for(size_t t = 0; t < trigger_count; ++t)
//    {
//      uintptr_t ghost_trigger(Data::trigger_get_trigger_data(trigger_data)[t].ghost_ptr);
//      
//      btPairCachingGhostObject *ghost_object(reinterpret_cast<btPairCachingGhostObject*>(ghost_trigger));
//      
//      if(!ghost_object)
//      {
//        LOG_ERROR_ONCE("Null trigger found.");
//        continue;
//      }
//      
//      const btBroadphasePairArray &pair_array(ghost_object->getOverlappingPairCache()->getOverlappingPairArray());
//      
//      const int pair_count(pair_array.size());
//      
//      for(int p = 0; p < pair_count; ++p)
//      {
//        const btBroadphasePair &pair(pair_array[p]);
//        const btBroadphasePair *collision_pair(dynamicsWorld->getPairCache()->findPair(pair.m_pProxy0, pair.m_pProxy1));
//        
//        if(!collision_pair)
//        {
//          continue;
//        }
//        
//        btManifoldArray manifold_array;
//        
//        if(collision_pair->m_algorithm)
//        {
//          collision_pair->m_algorithm->getAllContactManifolds(manifold_array);
//        }
//        
//        const int manifold_count = manifold_array.size();
//        
//        for(int m = 0; m < manifold_count; ++m)
//        {
//          const btPersistentManifold *manifold(manifold_array[m]);
//          const uintptr_t             entity_id_a((uintptr_t)manifold->getBody0()->getCollisionShape()->getUserPointer());
//          const uintptr_t             entity_id_b((uintptr_t)manifold->getBody1()->getCollisionShape()->getUserPointer());
//
//          #ifdef EXTRA_DEBUG_INFO_TRIGGER_CALLBACK
//          auto trans_00(manifold->getBody0()->getWorldTransform());
//          auto trans_01(manifold->getBody1()->getWorldTransform());
//          #endif
//          
//          if(!entity_id_a || !entity_id_b)
//          {
//            LOG_WARNING_ONCE("Found a collision shape with no user-data");
//            continue;
//          }
//          
//          const bool     is_first_body(manifold->getBody0() == ghost_object);
//          const btScalar direction(is_first_body ? -1.0 : 1.0);
//          
//          const int contact_count(manifold->getNumContacts());
//
//          for(int p = 0; p < contact_count; ++p)
//          {
//            const btManifoldPoint &pt(manifold->getContactPoint(p));
//            const float            penitration(pt.getDistance());
//            
//            if(penitration < 0.f)
//            {
//              #ifdef EXTRA_DEBUG_INFO_TRIGGER_CALLBACK
//              Core::Entity_ref ref_a(Core_detail::entity_id_from_uint(entity_id_a));
//              Core::Entity_ref ref_b(Core_detail::entity_id_from_uint(entity_id_b));
//              #endif
//              
//              const uint32_t collision_id(Data::collision_push(collision_data));
//              
//              Physics_transform::Collision_point contact
//              {
//                penitration * direction,
//                math::vec3_from_bt(pt.getPositionWorldOnA()),
//                math::vec3_from_bt(pt.m_normalWorldOnB)
//              };
//              
//              Data::collision_set_collision_point(collision_data, collision_id, &contact);
//              
//              const uint64_t entity_pair(lib::bits::pack3232(entity_id_a, entity_id_b));
//              Data::collision_set_entity_pair(collision_data, collision_id, &entity_pair);
//            }
//          } // for contact_count
//        } // for manifold_count
//      } // for pair_count
//    } // for trigger_count
  
    /*
      Non Trigger Collisions
      --
    */
    {
      btDynamicsWorld *dyn_world = phys->dynamics_world;
      btDispatcher *dispatcher   = dyn_world->getDispatcher();
      
      const int manifold_count = dispatcher->getNumManifolds();
      
      for(int i = 0; i < manifold_count; ++i)
      {
        const btPersistentManifold *contact_manifold(
          dispatcher->getManifoldByIndexInternal(i)
        );
        
        // Get Colliding Objects //
        uint32_t entity_id_a = 0;
        uint32_t entity_id_b = 0;
        {
          const btCollisionObject* obj_a(
            static_cast<const btCollisionObject*>(contact_manifold->getBody0())
          );

          const btCollisionObject* obj_b(
            static_cast<const btCollisionObject*>(contact_manifold->getBody1())
          );
          
          LIB_ASSERT(obj_a);
          LIB_ASSERT(obj_b);
          
          entity_id_a = (uint32_t)obj_a->getCollisionShape()->getUserPointer();
          entity_id_b = (uint32_t)obj_b->getCollisionShape()->getUserPointer();
        }
        LIB_ASSERT(entity_id_a);
        LIB_ASSERT(entity_id_b);

        // Get The Collision Info //
        const int contact_count = contact_manifold->getNumContacts();
        
        for (int j = 0; j < contact_count; j++)
        {
          const btManifoldPoint& pt = contact_manifold->getContactPoint(j);
          const float penitration = pt.getDistance();
          
          if(penitration < 0.f)
          {
//            const uint32_t collision_id(Data::collision_push(collision_data));
            
//            Physics_transform::Collision_point contact
//            {
//              penitration,
//              math::vec3_from_bt(pt.getPositionWorldOnA()),
//              math::vec3_from_bt(pt.m_normalWorldOnB)
//            };
//            
//            Data::collision_set_collision_point(collision_data, collision_id, &contact);
            
//            const uint64_t entity_pair(lib::bits::pack3232(entity_id_a, entity_id_b));
//            Data::collision_set_entity_pair(collision_data, collision_id, &entity_pair);

//            const uint64_t entity_pair(lib::bits::pack3232(entity_id_b, entity_id_a));
//            Data::collision_set_entity_pair(collision_data, collision_id, &entity_pair);
          }
        }
      }
    }

  
  /*
    Dispatch Collisions
    --
    Don't like the use of Core:: here.
    Where possible we should dump the information into an array, and let core apply
    the transformation to the data to generate Core::Collisions.
    
    Also don't like the callback here. World think should check if any collisions then fire its own callback.
  */
//  {
//    Data::Collision_data *collision_data(world->collision_data);
//    Data::Entity_data *entity_data(world->entity);
    
//    Data::data_lock(collision_data);
//    Data::data_lock(entity_data);
  
//    const uint32_t number_of_collisions(Data::collision_get_size(collision_data));
    
    
    
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
