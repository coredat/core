#ifndef DEBUG_RENDERER_INCLUDED_9A0B524E_6324_4994_8F34_86407D73A53A
#define DEBUG_RENDERER_INCLUDED_9A0B524E_6324_4994_8F34_86407D73A53A


#include "../../physics.hpp"
#include <btBulletDynamicsCommon.h>


namespace Bullet_detail {


struct Debug_drawer : public btIDebugDraw
{
  void
  drawLine(
    const btVector3& from,
    const btVector3& to,
    const btVector3& color);
  
  void
  drawContactPoint(
    const btVector3& PointOnB,
    const btVector3& normalOnB,
    btScalar distance,
    int lifeTime,
    const btVector3& color);
  
  void
  reportErrorWarning(const char* warningString);
  
  void
  draw3dText(const btVector3& location,
             const char* textString);
  
  void
  setDebugMode(int mode);
  
  int
  getDebugMode() const;
  
  int debug_mode = 0;
  Data::Physics::draw_line_fn draw_line_cb = nullptr;
  Data::Physics::draw_point_fn draw_point_cb = nullptr;
  
};


} // ns


#endif // inc guard
