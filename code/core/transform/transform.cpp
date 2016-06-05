#include "transform.hpp"
#include <math/vec/vec3.hpp>
#include <math/quat/quat.hpp>


namespace
{
  // The world we live in.
  const math::vec3 world_up      = math::vec3_init(0, +1, 0);
  const math::vec3 world_left    = math::vec3_init(+1, 0, 0);
  const math::vec3 world_forward = math::vec3_init(0, 0, -1);
}


namespace Core {


Transform::Transform(const math::vec3 pos,
                     const math::vec3 scale,
                     const math::quat rot)
: m_position(pos)
, m_scale(scale)
, m_rotation(rot)
{
}


Transform::Transform()
: Transform(math::vec3_zero(), math::vec3_one(), math::quat_init())
{
}


math::vec3
Transform::get_forward() const
{
  return math::quat_rotate_point(m_rotation, world_forward);
}


math::vec3
Transform::get_up() const
{
  return math::quat_rotate_point(m_rotation, world_up);
}


math::vec3
Transform::get_left() const
{
  return math::quat_rotate_point(m_rotation, world_left);
}


} // ns