#include <transformations/physics/update_world.hpp>
#include <core/physics/collision.hpp>
#include <utilities/logging.hpp>
#include <utilities/threading.hpp>


namespace
{
  uint32_t number_of_callbacks = 0;
  atomic_bool is_listening(false);
}


namespace Physics_transform {


void
update_world(q3Scene *scene,
             Core::Collision *out_collisions[],
             uint32_t *out_number_of_collisions)
{
  number_of_callbacks = 0;
  is_listening = true;
  
  LOG_TODO_ONCE("Use Scratch stream");
  
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
  
  is_listening = false;
}


} // ns