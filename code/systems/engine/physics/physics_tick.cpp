#include "physics_tick.hpp"

#include <transformations/entity/entity_transform.hpp>

#include <renderer/debug_line_renderer/debug_line_renderer_node.hpp>
#include <renderer/debug_line_renderer/debug_line_renderer.hpp>
#include <transformations/physics/bullet/bullet_math_extensions.hpp>
#include <transformations/physics/bullet/trigger_collisions.hpp>

#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include <BulletCollision/CollisionDispatch/btCollisionObject.h>

// Don't want core here! - Callback???
#include <core/common/collision.hpp>
#include <core/transform/transform.hpp>
#include <core/entity/detail/entity_id.hpp>
#include <core/entity/entity_components.hpp>

#include <data/world_data.hpp>
#include <data/world/rigidbody_data.hpp>


namespace {


// This state needs to go because its accross worlds
Core::Collision_callback callback_hack;

btPairCachingGhostObject *ghostObject;
btManifoldArray manifoldArray;

void ghost_callback(btDynamicsWorld *dynamicsWorld,
                    btScalar timeStep)
{
  btBroadphasePairArray& pairArray =
      ghostObject->getOverlappingPairCache()->getOverlappingPairArray();
  int numPairs = pairArray.size();
  
  for(int i = 0; i < numPairs; ++i)
  {
    const btBroadphasePair& pair = pairArray[i];

    btBroadphasePair* collisionPair =
        dynamicsWorld->getPairCache()->findPair(
            pair.m_pProxy0,pair.m_pProxy1);

    if (!collisionPair) continue;

    if (collisionPair->m_algorithm)
        collisionPair->m_algorithm->getAllContactManifolds(manifoldArray);

    for (int j=0;j<manifoldArray.size();j++)
    {
      btPersistentManifold* manifold = manifoldArray[j];
      auto ptr1 = (uintptr_t)manifold->getBody0()->getCollisionShape()->getUserPointer();
      auto ptr2 = (uintptr_t)manifold->getBody1()->getCollisionShape()->getUserPointer();

      bool isFirstBody = manifold->getBody0() == ghostObject;

      btScalar direction = isFirstBody ? btScalar(-1.0) : btScalar(1.0);

      for (int p = 0; p < manifold->getNumContacts(); ++p)
      {
        const btManifoldPoint&pt = manifold->getContactPoint(p);
        float pen = pt.getDistance();
      
        if (pen < 0.f)
        {
          const btVector3& ptA = pt.getPositionWorldOnA();
          const btVector3& ptB = pt.getPositionWorldOnB();
          const btVector3& normalOnB = pt.m_normalWorldOnB;

          // handle collisions here
        }
      }
    }
  }
}

} // anon ns


namespace Engine {
namespace Physics_tick {


void
initialize(std::shared_ptr<Data::World> world)
{
//  ghostObject = new btPairCachingGhostObject();
//  ghostObject->setCollisionShape(new btBoxShape(btVector3(btScalar(50.),btScalar(50.),btScalar(50.))));
//  ghostObject->setCollisionFlags(btGhostObject::CF_NO_CONTACT_RESPONSE);
//  ghostObject->getCollisionShape()->setUserPointer(ghostObject);
  
//  btTransform transform;
//  transform.setIdentity();
//  
//  ghostObject->setWorldTransform(transform);
  
//  world->dynamicsWorld->addCollisionObject(ghostObject);
  world->dynamicsWorld->getBroadphase()->getOverlappingPairCache()->setInternalGhostPairCallback(new btGhostPairCallback());
  
  world->dynamicsWorld->setInternalTickCallback(Physics_transform::trigger_callback, util::generic_id_to_ptr(world->world_instance_id), true);
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
      world->dynamicsWorld->stepSimulation(1 / 60.f, 10);
    }


  /*
    Collisions
    --
    Don't like the use of Core:: here.
    Where possible we should dump the information into an array, and let core apply
    the transformation to the data to generate Core::Collisions.
    
    Also don't like the callback here. World think should check if any collisions then fire its own callback.
  */
  {
    Data::Collision_data *collision_data(world->collision_data);
    Data::data_lock(collision_data);
  
    const uint32_t number_of_collisions(Data::collision_get_size(collision_data));
    
    if(number_of_collisions && callback_hack)
    {
      uint32_t curr_contact = 0;
      Core::Contact contacts[Core::Collision_detail::get_max_contacts()];
    
      while(curr_contact < number_of_collisions)
      {
        uint32_t contact_count = 0;
        const uint64_t curr_pair(collision_data->field_entity_pair[curr_contact]);
        
        const Core::Entity_ref entity_a(Core_detail::entity_id_from_uint(util::bits_lower(curr_pair)));
        const Core::Entity_ref entity_b(Core_detail::entity_id_from_uint(util::bits_upper(curr_pair)));
       
        // How many collisions for this pair
        while(true)
        {
          ++contact_count;
          if(collision_data->field_entity_pair[curr_contact + contact_count] != curr_pair)
          {
            break;
          }
        }
        
        // Generate Contact data
        const uint32_t contacts_to_gen = math::min(Core::Collision_detail::get_max_contacts(), contact_count);
        
        #ifndef NDEBUG
        if(contact_count < contacts_to_gen)
        {
          LOG_WARNING("Contacts have maxed out!");
        }
        #endif
        
        for(uint32_t c = 0; c < contacts_to_gen; ++c)
        {
          Physics_transform::Collision_point collision_pt(Data::collision_get_collision_point_data(collision_data)[curr_contact + c]);
        
          contacts[c] = Core::Contact(entity_b,
                                      collision_pt.point,
                                      collision_pt.normal,
                                      collision_pt.penitration);
        }

        Core::Collision collision(entity_a, contacts, contacts_to_gen);
        
        callback_hack(Core::Collision_type::enter, collision);
        
        curr_contact += contacts;
      }
    }
    
    Data::collision_clear(collision_data);
    Data::data_unlock(collision_data);
  }
  
  /*
    Update transforms
    --
    Now that we have shifted to bullet this should be done through the motion_states.
  */
  {
    // Set transforms.
    Data::Rigidbody_data *rb_data = world->rigidbody_data;
    
    for(size_t i = 0; i < Data::rigidbody_get_size(rb_data); ++i)
    {
      const uintptr_t rb_ptr = Data::rigidbody_get_rigidbody_data(rb_data)[i];
      
      if(rb_ptr)
      {
//        const q3Transform trans(reinterpret_cast<q3Body*>(rb_ptr)->GetTransform());
        btTransform trans;
        reinterpret_cast<btRigidBody*>(rb_ptr)->getMotionState()->getWorldTransform(trans);
        
        Core::Transform core_trans = math::transform_from_bt(trans);

        const uint32_t entity_id(rb_data->keys[i]);
        const Core::Entity_ref ref(Core_detail::entity_id_from_uint(entity_id));
        
        const Core::Transform old_trans(Core::Entity_component::get_transform(ref));
        
//        Core::Transform new_trans(to_core_trans(trans));
        core_trans.set_scale(old_trans.get_scale());

        
        Entity_detail::set_transform(entity_id,
                                     world->entity,
                                     world->transform,
                                     world->rigidbody_data,
                                     world->trigger_data,
                                     world->mesh_data,
                                     world->text_data,
                                     core_trans,
                                     false);
      }
    }
  }
}


void
de_initialize()
{
}


} // ns
} // ns