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
  // -- Param Check -- //
  LIB_ASSERT(phys);
  
  // -- Set Gravity -- //
  phys->dynamics_world->setGravity(
    math::vec3_to_bt(gravity)
  );
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
  
  /*
    Common var for getting trigger and rigidbody collisions.
  */
  constexpr size_t contact_stack_count = 64;
  using Contact_array = lib::array<Contact, contact_stack_count>;
  
  Contact_array frame_contacts;
  btDynamicsWorld *dyn_world = phys->dynamics_world;
  
  /*
    Trigger Collisions
  */
  {
    for(auto &trig : phys->trigger_data)
    {
      uintptr_t ghost_trigger(trig.ghost_ptr);
      
      btPairCachingGhostObject *ghost_object(
        reinterpret_cast<btPairCachingGhostObject*>(ghost_trigger)
      );
      
      if(!ghost_object)
      {
        LOG_ERROR_ONCE("Null trigger found.");
        continue;
      }
    
      struct callback : public btCollisionWorld::ContactResultCallback
      {
        btScalar
        addSingleResult(
          btManifoldPoint &manifold,
          const btCollisionObjectWrapper* collision_shape_a,
          int partId0,
          int index0,
          const btCollisionObjectWrapper* collision_shape_b,
          int partId1,
          int index1) override
        {
          // -- Param Check -- //
          LIB_ASSERT(collision_shape_a);
          LIB_ASSERT(collision_shape_b);
          LIB_ASSERT(this->trigger_contacts);
        
          // -- Design Error -- //
          // http://bulletphysics.org/Bullet/phpBB3/viewtopic.php?t=5982#p20990
          constexpr btScalar unused_return = 1.f;
        
          // -- Get Entities -- //
          const uint32_t entity_id_a = lib::key::id_from_ptr(
            collision_shape_a->getCollisionShape()->getUserPointer()
          );
          
          const uint32_t entity_id_b = lib::key::id_from_ptr(
            collision_shape_b->getCollisionShape()->getUserPointer()
          );

          if(!entity_id_a || !entity_id_b)
          {
            LOG_WARNING_ONCE("Found a collision shape with no user-data");
            return unused_return;
          }
          
          const float penitration(manifold.getDistance());
          
          if(penitration < 0.f)
          {
            const float direction = 1.f;
            
            trigger_contacts->emplace_back(
              math::vec3_from_bt(manifold.getPositionWorldOnA()),
              math::vec3_from_bt(manifold.m_normalWorldOnB),
              penitration * direction,
              lib::bits::pack3232(entity_id_a, entity_id_b)
            );
          }
          
          return unused_return;
        }
        
        Contact_array *trigger_contacts = nullptr;
      };

      callback cb;
      cb.trigger_contacts = &frame_contacts;

      dyn_world->contactTest(ghost_object, cb);
    }
  }
  
  /*
    Non Trigger Collisions
    --
  */
  {
    btDispatcher *dispatcher = dyn_world->getDispatcher();
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
        
        entity_id_a = lib::key::id_from_ptr(
          obj_a->getCollisionShape()->getUserPointer()
        );
        
        entity_id_b = lib::key::id_from_ptr(
          obj_b->getCollisionShape()->getUserPointer()
        );
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
          frame_contacts.emplace_back(
            math::vec3_from_bt(pt.getPositionWorldOnA()),
            math::vec3_from_bt(pt.m_normalWorldOnB),
            penitration,
            lib::bits::pack3232(entity_id_a, entity_id_b)
          );
          
          frame_contacts.emplace_back(
            math::vec3_from_bt(pt.getPositionWorldOnB()),
            math::vec3_scale(math::vec3_from_bt(pt.m_normalWorldOnB), -1),
            penitration,
            lib::bits::pack3232(entity_id_b, entity_id_a)
          );
        }
      }
    }
  } // Rigidbody Collisions with other rigidbodies.
  
  /*
    Process the contacts
    --
    Will need to process enter/exit right now just push them into current.
  */
  {
    #ifndef NDEBUG
    const size_t number_of_contacts = frame_contacts.size();
    if(number_of_contacts > contact_stack_count)
    
    {
      LOG_INFO_ONCE(
        "Contacts broke the allocated stack size, heap was used."
      );
    }
    #endif
  
    phys->collisions_current.clear();
    
    for(const auto &frame : frame_contacts)
    {
      phys->collisions_current.emplace_back(frame);
    }
  }
}


size_t
world_get_number_of_colliding_items(const Physics_data *phys)
{
  return phys->collisions_current.size();
}


const Contact*
world_get_colliding_items(const Physics_data *phys)
{
  return phys->collisions_current.data();
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
  
    const uint32_t entity_id(
      lib::key::id_from_ptr(
        ray_callback.m_collisionObject->getCollisionShape()->getUserPointer()
      )
    );
    
    out_contacts[0].contact_point  = math::vec3_from_bt(ray_callback.m_hitPointWorld);
    out_contacts[0].contact_normal = math::vec3_from_bt(ray_callback.m_hitNormalWorld);
    out_contacts[0].distance       = length;
    out_contacts[0].collision_a_b  = lib::bits::pack3232(0, entity_id);
    
    *out_number_of_contacts = 1;
  }
}


} // ns
} // ns
