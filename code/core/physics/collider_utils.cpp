#include <core/physics/collider_utils.hpp>
#include <core/physics/collider.hpp>
#include <core/physics/box_collider.hpp>
#include <math/vec/vec3.hpp>


namespace Core {
namespace Collider_utis {


Box_collider
cast_to_box_collider(const Collider coll)
{
  return Box_collider(coll.get_arg_01(), coll.get_arg_02(), coll.get_arg_03());
}


} // ns
} // ns