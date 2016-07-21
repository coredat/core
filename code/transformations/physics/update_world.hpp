#ifndef UPDATE_WORLD_INCLUDED_FAE0D2E9_F8CA_4395_8FA3_ACF2E109CCED
#define UPDATE_WORLD_INCLUDED_FAE0D2E9_F8CA_4395_8FA3_ACF2E109CCED


#include <core/physics/physics_fwd.hpp>
#include <3rdparty/qu3e/q3.h>
#include <stdint.h>


namespace Physics_transform {


void
update_world(q3Scene *scene,
             Core::Collision *out_collisions[],
             uint32_t *out_number_of_collisions = nullptr);


} // ns


#endif // inc guard