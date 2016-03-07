#ifndef PHYSICS_ENGINE_INCLUDED_937163DD_0E64_4054_85E9_C6415B66AE1A
#define PHYSICS_ENGINE_INCLUDED_937163DD_0E64_4054_85E9_C6415B66AE1A


#include <math/geometry/geometry.hpp>


namespace Physics_engine {


void
initialize();


bool
does_aabb_collide();


bool
add_aabb(const math::aabb);


} // ns


#endif // inc guardt