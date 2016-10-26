#ifndef TRIGGER_COLLISIONS_INCLUDED_A8E07F52_7CB8_4F79_BA1F_DEBFB977AF2E
#define TRIGGER_COLLISIONS_INCLUDED_A8E07F52_7CB8_4F79_BA1F_DEBFB977AF2E


#include <LinearMath/btScalar.h>

class btDynamicsWorld;


namespace Physics_transform {

/*!
  This will go through all the transforms of a world
  and check for collisions, it will add them to the collision_data model.
  
  This will not include any collisions from rigidbodies.
*/
void
trigger_callback(btDynamicsWorld *dynamicsWorld,
                 btScalar timeStep);

} // ns


#endif // inc guard