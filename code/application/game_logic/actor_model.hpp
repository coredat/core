#ifndef ACTOR_BASE_INCLUDED_67983ACD_4D6D_4A09_9F50_DB189567416C
#define ACTOR_BASE_INCLUDED_67983ACD_4D6D_4A09_9F50_DB189567416C


#include "../common/ids_component_types.hpp"
#include <core/interface/component.hpp>
#include <math/vec/vec3.hpp>
#include <btBulletCollisionCommon.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include <memory>


/*
  TODO
  --
  Consider this
  We will have many actors! 10 - 40?
  We shall need ai on a nav mesh!
  
  We should data this, when it settles down.
*/


class Actor_model : public Core::Component
{
private:

  // TODO Actor controller?
  friend class Npc_controller;
  friend class Network_player_controller;
  friend class Local_player_controller;
  
  uint32_t              get_rtti() const override { return Component_type::actor; }
  
  void                  on_start() override;
  void                  on_end() override;
  void                  on_update(const float dt) override;
  void                  on_event(const uint32_t id, const void *data, const uint32_t size_of_data) override;
  
  void                  move_forward(const float fwd);
  void                  move_left(const float left);
  void                  look_up(const float pitch);
  void                  turn_left(const float left);
  bool                  is_grounded() const;
  
  void                  jump();
  void                  crouch();
  void                  take_damage();
  
private:

  math::vec3            m_pending_move          = math::vec3_zero();
  math::vec3            m_acuumulated_rotations = math::vec3_zero();
  math::vec3            m_size                  = math::vec3_init(0.5, 2, 0.5);
  const float           m_gravity               = -0.5f;
  float                 m_max_health            = 10.f;
  float                 m_health                = 10.f;
  
  // Urgh these need to go into the rb system unfort.
  std::unique_ptr<btPairCachingGhostObject> m_ghost_obj;
  std::unique_ptr<btGhostPairCallback> m_pair_cb;
  std::unique_ptr<btCollisionShape> m_collisionshape;
  
}; // class


#endif // inc guard