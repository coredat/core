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
: Transform(math::vec3_zero(),
            math::vec3_one(),
            math::quat_init())
{
}


math::vec3
Transform::get_position() const
{
  return m_position;
}


void
Transform::set_position(const math::vec3 pos)
{
  m_position = pos;
}


math::quat
Transform::get_rotation() const
{
  return m_rotation;
}


void
Transform::set_rotation(const math::quat rot)
{
  m_rotation = rot;
}


math::vec3
Transform::get_scale() const
{
  return m_scale;
}


void
Transform::set_scale(const math::vec3 scale)
{
  m_scale = scale;
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


math::vec3
Transform::get_world_forward()
{
  return world_forward;
}
  

math::vec3
Transform::get_world_up()
{
  return world_up;
}


math::vec3
Transform::get_world_left()
{
  return world_left;
}


} // ns