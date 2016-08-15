#include <core/transform/transform_utils.hpp>
#include <core/transform/transform.hpp>
#include <math/transform/transform.hpp>
#include <math/mat/mat4.hpp>
#include <math/vec/vec3.hpp>


namespace
{
  // The world we live in.
  const math::vec3 world_up      = math::vec3_init(0, +1, 0);
  const math::vec3 world_left    = math::vec3_init(+1, 0, 0);
  const math::vec3 world_forward = math::vec3_init(0, 0, -1);
}


namespace Core {
namespace Transform_utils {


math::vec3
get_world_fwd()
{
  return world_forward;
}


math::vec3
get_world_up()
{
  return world_up;
}


math::vec3
get_world_left()
{
  return world_left;
}


math::mat4
get_world_matrix(const Core::Transform &transform)
{
  const math::transform trans = math::transform_init(transform.get_position(),
                                                     transform.get_scale(),
                                                     transform.get_rotation());
  
  return math::transform_get_world_matrix(trans);
}


} // ns
} // ns