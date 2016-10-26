#include <transformations/physics/bullet/trigger_collisions.hpp>
#include <transformations/physics/bullet/bullet_math_extensions.hpp>
#include <btBulletDynamicsCommon.h>
#include <BulletDynamics/btBulletDynamicsCommon.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include <BulletCollision/CollisionDispatch/btCollisionObject.h>

#include <core/world/detail/world_index.hpp>
#include <core/entity/detail/entity_id.hpp>

#include <data/world/collision_data.hpp>
#include <data/world/trigger_data.hpp>
#include <data/world_data.hpp>
#include <utilities/logging.hpp>
#include <utilities/generic_id.hpp>
#include <utilities/bits.hpp>


//#define EXTRA_DEBUG_INFO_TRIGGER_CALLBACK

#ifdef EXTRA_DEBUG_INFO_TRIGGER_CALLBACK
#include <core/entity/entity_ref.hpp>
#endif


namespace Physics_transform {


void
trigger_callback(btDynamicsWorld *dynamicsWorld,
                 btScalar timeStep)
{
  // Param Check
  assert(dynamicsWorld);
  assert(dynamicsWorld->getWorldUserInfo());

  // Data Get / Check
  const util::generic_id world_id = util::generic_id_from_ptr(dynamicsWorld->getWorldUserInfo());
  
  if(!world_id)
  {
    LOG_ERROR("No world id found");
    return;
  }
  
  auto                   world_data(Core_detail::world_index_get_world_data(world_id));
  Data::Collision_data  *collision_data(world_data->collision_data);
  Data::Trigger_data    *trigger_data(world_data->trigger_data);
  
  if(!world_data || !collision_data || !trigger_data)
  {
    LOG_FATAL("Missing Data");
    return;
  }
  
  /*
    Trigger Collisions
    --
    For each trigger check overlapping pairs.
  */
  {
    Data::data_lock(collision_data);
    Data::data_lock(trigger_data);

    const size_t trigger_count(Data::trigger_get_size(trigger_data));
    
    for(size_t t = 0; t < trigger_count; ++t)
    {
      uintptr_t ghost_trigger(Data::trigger_get_trigger_data(trigger_data)[t]);
      
      btPairCachingGhostObject *ghost_object(reinterpret_cast<btPairCachingGhostObject*>(ghost_trigger));
      
      if(!ghost_object)
      {
        LOG_ERROR_ONCE("Null trigger found.");
        continue;
      }
      
      const btBroadphasePairArray &pair_array(ghost_object->getOverlappingPairCache()->getOverlappingPairArray());
      
      const int pair_count(pair_array.size());
      
      for(int p = 0; p < pair_count; ++p)
      {
        const btBroadphasePair &pair(pair_array[p]);
        const btBroadphasePair *collision_pair(dynamicsWorld->getPairCache()->findPair(pair.m_pProxy0, pair.m_pProxy1));
        
        if(!collision_pair)
        {
          continue;
        }
        
        btManifoldArray manifold_array;
        
        if(collision_pair->m_algorithm)
        {
          collision_pair->m_algorithm->getAllContactManifolds(manifold_array);
        }
        
        const int manifold_count = manifold_array.size();
        
        for(int m = 0; m < manifold_count; ++m)
        {
          const btPersistentManifold *manifold(manifold_array[m]);
          const uintptr_t             entity_id_a((uintptr_t)manifold->getBody0()->getCollisionShape()->getUserPointer());
          const uintptr_t             entity_id_b((uintptr_t)manifold->getBody1()->getCollisionShape()->getUserPointer());

          #ifdef EXTRA_DEBUG_INFO_TRIGGER_CALLBACK
          auto trans_00(manifold->getBody0()->getWorldTransform());
          auto trans_01(manifold->getBody1()->getWorldTransform());
          #endif
          
          if(!entity_id_a || !entity_id_b)
          {
            LOG_WARNING_ONCE("Found a collision shape with no user-data");
            continue;
          }
          
          const bool     is_first_body(manifold->getBody0() == ghost_object);
          const btScalar direction(is_first_body ? -1.0 : 1.0);
          
          const int contact_count(manifold->getNumContacts());

          for(int p = 0; p < contact_count; ++p)
          {
            const btManifoldPoint &pt(manifold->getContactPoint(p));
            const float            penitration(pt.getDistance());
            
            if(penitration < 0.f)
            {
              #ifdef EXTRA_DEBUG_INFO_TRIGGER_CALLBACK
              Core::Entity_ref ref_a(Core_detail::entity_id_from_uint(entity_id_a));
              Core::Entity_ref ref_b(Core_detail::entity_id_from_uint(entity_id_b));
              #endif
              
              const auto collision_id(Data::collision_push(collision_data));
              
              Physics_transform::Collision_point contact
              {
                penitration * direction,
                math::vec3_from_bt(pt.getPositionWorldOnA()),
                math::vec3_from_bt(pt.m_normalWorldOnB)
              };
              
              Data::collision_set_collision_point(collision_data, collision_id, &contact);
              
              const uint64_t entity_pair(util::bits_pack(entity_id_a, entity_id_b));
              Data::collision_set_entity_pair(collision_data, collision_id, &entity_pair);
            }
          } // for contact_count
        } // for manifold_count
      } // for pair_count
    } // for trigger_count
    Data::data_unlock(trigger_data);
    Data::data_unlock(collision_data);
  }
}


} // ns
