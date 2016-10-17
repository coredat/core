#ifndef UPDATE_WORLD_INCLUDED_FAE0D2E9_F8CA_4395_8FA3_ACF2E109CCED
#define UPDATE_WORLD_INCLUDED_FAE0D2E9_F8CA_4395_8FA3_ACF2E109CCED


#include <core/physics/physics_fwd.hpp>
#include <data/world_data.hpp>
#include <memory>
#include <stdint.h>


namespace Physics_transform {


void
update_world(std::shared_ptr<Data::World> world,
             Core::Collision_pair *out_collisions[] = nullptr,
             uint32_t *out_number_of_collisions = nullptr);


} // ns


#endif // inc guard