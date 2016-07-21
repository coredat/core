#include <transformations/physics/update_world.hpp>


namespace
{
  uint32_t number_of_callbacks = 0;
}


namespace Physics_transform {


void
update_world(q3Scene *scene,
             uint32_t *out_number_of_collisions)
{
  number_of_callbacks = 0;

  struct callback : public q3ContactListener
  {
    void BeginContact(const q3ContactConstraint *contact)
    {
      ++number_of_callbacks;
    }
    
    void EndContact(const q3ContactConstraint *contact)
    {
      ++number_of_callbacks;
    }
  };
  
  if(out_number_of_collisions)
  {
  }
  
  static callback cb;
  scene->SetContactListener(&cb);
  scene->SetIterations(100);

  scene->Step();
  
  if(out_number_of_collisions)
  {
    *out_number_of_collisions = number_of_callbacks;
  }
}


} // ns