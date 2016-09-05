#include <transformations/physics/update_world.hpp>
#include <core/physics/collision.hpp>
#include <data/global_data/memory_data.hpp>
#include <utilities/logging.hpp>
#include <utilities/threading.hpp>
#include <3rdparty/qu3e/dynamics/q3Contact.h>
#include <3rdparty/qu3e/dynamics/q3Body.h>


namespace
{
  uint32_t number_of_callbacks = 0;
  atomic_bool is_listening(false);
  Core::Collision *collisions = nullptr;
  std::shared_ptr<World_data::World> world; // this is a bad bad hack to smuggle world into the callback.
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
update_world(std::shared_ptr<World_data::World> curr_world,
             Core::Collision *out_collisions[],
             uint32_t *out_number_of_collisions)
{
  number_of_callbacks = 0;
  is_listening = true;
  world = curr_world;
  
  LOG_TODO_ONCE("Use Scratch stream");
  
  collisions = SCRATCH_ALIGNED_ALLOC(Core::Collision, 2048);
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
      
      Core::Entity_ref coll_ent_a(util::generic_id_from_ptr(contact->bodyA->GetUserData()), world);
      Core::Entity_ref coll_ent_b(util::generic_id_from_ptr(contact->bodyB->GetUserData()), world);
      
      assert(coll_ent_a.is_valid());
      assert(coll_ent_b.is_valid());
      
      // Make two collisions for each entity.
      new(&collisions[number_of_callbacks++]) Core::Collision(coll_ent_a,
                                                              coll_ent_b,
                                                              math::vec3_from_qu3e(contact->manifold.normal),
                                                              0);
      
      new(&collisions[number_of_callbacks++]) Core::Collision(coll_ent_b,
                                                              coll_ent_a,
                                                              math::vec3_scale(math::vec3_from_qu3e(contact->manifold.normal), -1.f),
                                                              0);
          
    }
    
    /*
      These get called from qu3e.
    */
    void BeginContact(const q3ContactConstraint *contact) override  { Register_contact(contact); }
    void EndContact(const q3ContactConstraint *contact) override    { Register_contact(contact); }
  };
  
  static callback cb;
  
  auto &phys_scene = world->scene;
  
  phys_scene->SetContactListener(&cb);
  phys_scene->SetIterations(5);
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