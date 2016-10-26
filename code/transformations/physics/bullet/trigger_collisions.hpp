#ifndef TRIGGER_COLLISIONS_INCLUDED_A8E07F52_7CB8_4F79_BA1F_DEBFB977AF2E
#define TRIGGER_COLLISIONS_INCLUDED_A8E07F52_7CB8_4F79_BA1F_DEBFB977AF2E


#include <LinearMath/btScalar.h>

class btDynamicsWorld;


namespace Physics_transform {


void
trigger_callback(btDynamicsWorld *dynamicsWorld,
                 btScalar timeStep);

} // ns


#endif // inc guard