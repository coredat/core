#include "physics_tick.hpp"

#include <transformations/entity/entity_transform.hpp>

#include <renderer/debug_line_renderer/debug_line_renderer_node.hpp>
#include <renderer/debug_line_renderer/debug_line_renderer.hpp>
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




namespace {


// This state needs to go because its accross worlds
Core::Collision_callback callback_hack;

} // anon ns


namespace Engine {
namespace Physics_tick {


void
initialize(std::shared_ptr<Data::World> world)
{
  world->dynamicsWorld->getBroadphase()->getOverlappingPairCache()->setInternalGhostPairCallback(new btGhostPairCallback());
  
  world->dynamicsWorld->setInternalTickCallback(Physics_transform::trigger_callback,
                                                util::generic_id_to_ptr(world->world_instance_id),
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
      world->dynamicsWorld->stepSimulation(1 / 60.f, 500);
    }

  
  /*
    Check collisions
  */
  {
  
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
      uint32_t curr_contact(0);
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
  


//int numManifolds = world->getDispatcher()->getNumManifolds();
//    for (int i=0;i<numManifolds;i++)
//    {
//        btPersistentManifold* contactManifold =  world->getDispatcher()->getManifoldByIndexInternal(i);
//        btCollisionObject* obA = static_cast<btCollisionObject*>(contactManifold->getBody0());
//        btCollisionObject* obB = static_cast<btCollisionObject*>(contactManifold->getBody1());
//
//        int numContacts = contactManifold->getNumContacts();
//        for (int j=0;j<numContacts;j++)
//        {
//            btManifoldPoint& pt = contactManifold->getContactPoint(j);
//            if (pt.getDistance()<0.f)
//            {
//                const btVector3& ptA = pt.getPositionWorldOnA();
//                const btVector3& ptB = pt.getPositionWorldOnB();
//                const btVector3& normalOnB = pt.m_normalWorldOnB;
//            }
//        }
//    }
}


void
de_initialize()
{
}


} // ns
} // ns