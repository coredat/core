#include "debug_renderer.hpp"
#include "bullet_math_extensions.hpp"
#include <math/math.hpp>
#include <utilities/logging.hpp>


namespace Bullet_detail {


void
Debug_drawer::drawLine(
  const btVector3& from,
  const btVector3& to,
  const btVector3& color)
{
  if(draw_line_cb)
  {
    draw_line_cb(
      math::vec3_from_bt(from),
      math::vec3_from_bt(to),
      math::vec3_from_bt(color)
    );
  }
}

void
Debug_drawer::drawContactPoint(
  const btVector3& PointOnB,
  const btVector3& normalOnB,
  btScalar distance,
  int lifeTime,
  const btVector3& color)
{
  if(draw_point_cb)
  {
    draw_point_cb(
      math::vec3_from_bt(PointOnB),
      math::vec3_from_bt(color)
    );
  }
}

void
Debug_drawer::reportErrorWarning(const char* warningString)
{
  LOG_WARNING(warningString);
}

void
Debug_drawer::draw3dText(
  const btVector3& location,
  const char* textString)
{
}

void
Debug_drawer::setDebugMode(int mode)
{
  debug_mode = mode;
}

int
Debug_drawer::getDebugMode() const
{
  return debug_mode;
}


} // ns
