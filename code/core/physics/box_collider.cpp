#include <core/physics/box_collider.hpp>
#include <core/physics/box_collider_utils.hpp>
#include <core/physics/collider.hpp>


namespace Core {


Box_collider::operator Collider() const
{
  return Box_collider_utils::cast_to_collider(*this);
}


} // ns