#ifndef BOUNDS_UTILS_INCLUDED_B526F540_56AA_4DFA_89E2_0806AFDB6284
#define BOUNDS_UTILS_INCLUDED_B526F540_56AA_4DFA_89E2_0806AFDB6284


#include <core/common/bounds.hpp>


namespace Core {
namespace Bound_utils {


/*!
  Creats a bounding area from an origin and extent.
*/
Bounds
create_with_origin_extents(const math::vec3 origin, const math::vec3 extents);


/*!
  Gets the origin of the bound area.
*/
math::vec3
get_origin(const Bounds &bounds);


/*!
  Gets the full extent of the bounds.
  This is the size.
*/
math::vec3
get_extents(const Bounds &bounds);


/*!
  Gets the half extent of the bounds.
  This is half the size.
*/
math::vec3
get_half_extents(const Bounds &bounds);


} // ns
} // ns


#endif // inc guard