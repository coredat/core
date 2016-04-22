#ifndef COLLIDER_UTILS_INCLUDED_DEB9644D_6590_4797_9C1B_8384AA3CDFD6
#define COLLIDER_UTILS_INCLUDED_DEB9644D_6590_4797_9C1B_8384AA3CDFD6


#include <core/physics/physics_fwd.hpp>
#include <math/vec/vec_types.hpp>


namespace Core {
namespace Collider_utis {


Box_collider
cast_to_box_collider(const Collider collider);


Collider
cast_to_collider(const Box_collider collider);


} // ns
} // ns


#endif // inc gaurd