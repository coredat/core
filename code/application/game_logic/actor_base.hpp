#ifndef ACTOR_BASE_INCLUDED_67983ACD_4D6D_4A09_9F50_DB189567416C
#define ACTOR_BASE_INCLUDED_67983ACD_4D6D_4A09_9F50_DB189567416C


#include <core/interface/component.hpp>
#include <math/vec/vec3.hpp>
#include <btBulletCollisionCommon.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include <memory>


class Actor_base : public Core::Component
{
public:

  explicit              Actor_base();
  virtual               ~Actor_base();
  
  virtual void          on_start();
  virtual void          on_update(const float dt) override;
  
  void                  move_forward(const float fwd);
  void                  move_left(const float left);
  void                  look_up(const float pitch);
  void                  turn_left(const float left);
  
  bool                  is_grounded() const;
  
private:

  math::vec3            m_pending_move = math::vec3_zero();
  math::vec3            m_acuumulated_rotations = math::vec3_zero();
  math::vec3            m_size = math::vec3_init(0.5, 2, 0.5);
  const float           m_gravity = -0.5f;
  
  
  std::unique_ptr<btPairCachingGhostObject> m_ghost_obj;
  std::unique_ptr<btGhostPairCallback> m_pair_cb;
  std::unique_ptr<btCylinderShape> m_collisionshape;
  
}; // class


#endif // inc guard