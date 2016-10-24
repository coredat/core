#ifndef FIND_BY_RAY_INCLUDED_F444A6FA_4324_4739_8F91_C69FE4155E6B
#define FIND_BY_RAY_INCLUDED_F444A6FA_4324_4739_8F91_C69FE4155E6B


#include <core/common/core_fwd.hpp>


class btDiscreteDynamicsWorld;


namespace Physics_transform {


/*
  Find an entity from a given ray.
*/
Core::Collision
find_entity_from_ray(const Core::Ray ray,
                     btDiscreteDynamicsWorld *phy_world);


} // ns


#endif // inc guard