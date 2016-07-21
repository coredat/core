#include <transformations/physics/update_world.hpp>
#include <core/physics/collision.hpp>
#include <data/global_data/memory_data.hpp>
#include <utilities/logging.hpp>
#include <utilities/threading.hpp>
#include <3rdparty/qu3e/dynamics/q3Contact.h>


namespace
{
  uint32_t number_of_callbacks = 0;
  atomic_bool is_listening(false);
  Core::Collision *collisions = nullptr;
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
update_world(q3Scene *scene,
             Core::Collision *out_collisions[],
             uint32_t *out_number_of_collisions)
{
  number_of_callbacks = 0;
  is_listening = true;
  
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
      
      collisions[number_of_callbacks] = Core::Collision(Core::Entity_ref(),
                                                        Core::Entity_ref(),
                                                        math::vec3_from_qu3e(contact->manifold.normal),
                                                        0);
      ++number_of_callbacks;
    }
    
    /*
      These get called from qu3e.
    */
    void BeginContact(const q3ContactConstraint *contact) override  { Register_contact(contact); }
    void EndContact(const q3ContactConstraint *contact) override    { Register_contact(contact); }
  };
  
  static callback cb;
  scene->SetContactListener(&cb);
  scene->SetIterations(100);
  
  scene->Step();
  
  if(out_number_of_collisions)
  {
    *out_number_of_collisions = number_of_callbacks;
  }
  
  if(out_collisions && collisions)
  {
    *out_collisions = collisions;
  }
  
  is_listening = false;
}


} // ns