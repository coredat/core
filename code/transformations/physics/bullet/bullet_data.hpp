#ifndef BULLET_DATA_INCLUDED_84ABBDE9_4582_4635_89BC_3F40B09E03F6
#define BULLET_DATA_INCLUDED_84ABBDE9_4582_4635_89BC_3F40B09E03F6


#include <stdint.h>
#include <btBulletDynamicsCommon.h>
#include <BulletCollision/Gimpact/btGImpactCollisionAlgorithm.h>


class btDynamicsWorld;


namespace Bullet_data {


/*!
  Bullet works on 'you allocate you destroy' philosophy
  --
  These are all the allocations for a rigidbody.
  --
  We store them as uintptr_t's because this structure is used
  inside a model, and by convention thats how we store raw pointers.
*/
struct Rigidbody
{
  uintptr_t rigidbody_ptr    = 0;
  uintptr_t shape_ptr        = 0;
  uintptr_t motion_state_ptr = 0;
};


/*!
  Bullet works on 'you allocate you destroy' philosophy
  --
  These are all the allocations for a trigger.
  --
  We store them as uintptr_t's because this structure is used
  inside a model, and by convention thats how we store raw pointers.
*/
struct Trigger
{
  uintptr_t ghost_ptr = 0;
  uintptr_t shape_ptr = 0;
};


struct World
{
  btBroadphaseInterface               *broadphase       = nullptr;
  btDefaultCollisionConfiguration     *collision_config = nullptr;
  btCollisionDispatcher               *dispatcher       = nullptr;
  btSequentialImpulseConstraintSolver *solver           = nullptr;
  btDiscreteDynamicsWorld             *dynamics_world   = nullptr;
};


/*!
  Removes the rigidbody from the physics world, deletes the allocated resources.
  --
  This should be the ONLY way to delete/remove a rigidbody.
*/
void
remove_and_clear(Rigidbody *rb, btDynamicsWorld *world);


/*!
  Removes the trigger from the physics world, deletes the allocated resources.
  --
  This should be the ONLY way to delete/remove a trigger.
*/
void
remove_and_clear(Trigger *trigger, btDynamicsWorld *world);


/*!
  Setup the the physics world
*/
void
setup(World *world);


/*!
  Destroys the world
*/
void
remove_and_clear(World *world);



} // ns


#endif // inc guard