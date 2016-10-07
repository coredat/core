#ifndef COLLISION_INCLUDED_439C50AF_1E6B_470F_A8D5_C95C16251E04
#define COLLISION_INCLUDED_439C50AF_1E6B_470F_A8D5_C95C16251E04


#include <core/common/core_types.hpp>
#include <core/common/core_fwd.hpp>
#include <core/common/collision.hpp>


namespace Core {


/*
  Pair of entities that collided with each other.
*/
struct Collision_pair
{
  Collision with_this;
  Collision with_that;
};


} // ns


#endif // inc guard