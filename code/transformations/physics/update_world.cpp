#include <transformations/physics/update_world.hpp>
#include <data/world/rigidbody_data.hpp>
#include <core/entity/detail/entity_id.hpp>
#include <core/physics/collision.hpp>
#include <core/common/collision.hpp>
#include <data/memory/memory_data.hpp>
#include <utilities/logging.hpp>
#include <utilities/threading.hpp>
#include <3rdparty/qu3e/dynamics/q3Contact.h>
#include <3rdparty/qu3e/dynamics/q3Body.h>


namespace
{
  uint32_t number_of_callbacks = 0;
  atomic_bool is_listening(false);
  Core::Collision_pair *collisions = nullptr;
  std::shared_ptr<Data::World> world; // this is a bad bad hack to smuggle world into the callback.
}


namespace math {

  math::vec3
  vec3_from_qu3e(const q3Vec3 vec)
  {
    return math::vec3_init(vec.x, vec.y, vec.z);
  }
  

} // ns


namespace Physics_transform {


void
update_world(std::shared_ptr<Data::World> curr_world,
             Core::Collision_pair *out_collisions[],
             uint32_t *out_number_of_collisions)
{
  number_of_callbacks = 0;
  is_listening = true;
  world = curr_world;
  
  LOG_TODO_ONCE("Use A data pool");
  
  collisions = SCRATCH_ALIGNED_ALLOC(Core::Collision_pair, 2048);
  memset(collisions, 0, sizeof(collisions) * 2048);
  
  // Generate an arrray for results.
  // Set that as an output.

  struct callback : public q3ContactListener
  {
    /*
      Register the collision.
    */
    void
    Register_contact(const q3ContactConstraint *contact)
    {
      if(!is_listening)
      {
        LOG_WARNING_ONCE("Ignoring collision callback.");
        return;
      }
      
      if(number_of_callbacks >= 2048)
      {
        LOG_ERROR("too many collisions");
      }
      
      const auto user_data_a = util::generic_id_from_ptr(contact->bodyA->GetUserData());
      const Core::Entity_ref coll_ent_a(Core_detail::entity_id_from_uint(user_data_a));
      assert(coll_ent_a.is_valid());
      
      const auto user_data_b = util::generic_id_from_ptr(contact->bodyB->GetUserData());
      const Core::Entity_ref coll_ent_b(Core_detail::entity_id_from_uint(user_data_b));
      assert(coll_ent_b.is_valid());
      
      static Core::Contact contacts_a[Core::Collision_detail::get_max_contacts()];
      static Core::Contact contacts_b[Core::Collision_detail::get_max_contacts()];
      
      const size_t contact_count = math::min(Core::Collision_detail::get_max_contacts(),
                                             static_cast<decltype(Core::Collision_detail::get_max_contacts())>(contact->manifold.contactCount));
      
      for(uint32_t i = 0; i < contact_count; ++i)
      {
        const q3Manifold &m = contact->manifold;
        const q3Contact &c = contact->manifold.contacts[i];
        
        contacts_a[i] = Core::Contact(
          math::vec3_from_qu3e(c.position),
          math::vec3_from_qu3e(m.normal),
          c.penetration
        );
        
        contacts_b[i] = Core::Contact(
          math::vec3_from_qu3e(c.position),
          math::vec3_scale(math::vec3_from_qu3e(m.normal), -1.f),
          c.penetration
        );
      }
      
      new(&collisions[number_of_callbacks++]) Core::Collision_pair{
        Core::Collision(coll_ent_a, contacts_a, contact_count),
        Core::Collision(coll_ent_b, contacts_b, contact_count)
      };
    }
    
    /*
      These get called from qu3e.
    */
    void BeginContact(const q3ContactConstraint *contact) override  { Register_contact(contact); }
    void EndContact(const q3ContactConstraint *contact) override    { Register_contact(contact); }
  };
  
  static callback *cb = nullptr;
  
  
  auto &phys_scene = world->scene;
  
  if(!cb)
  {
    cb = new callback();
    phys_scene->SetContactListener(cb);
    phys_scene->SetIterations(60);
  }
  
  phys_scene->Step();
  
  if(out_number_of_collisions)
  {
    *out_number_of_collisions = number_of_callbacks;
  }
  
  if(out_collisions && collisions)
  {
    *out_collisions = collisions;
  }
  
  is_listening = false;
  world.reset();
}


} // ns