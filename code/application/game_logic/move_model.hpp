#ifndef MOVE_MODEL_INCLUDED_501F5089_FD6E_4962_99FF_704D683C518B
#define MOVE_MODEL_INCLUDED_501F5089_FD6E_4962_99FF_704D683C518B


#include "../common/ids_component_types.hpp"
#include <core/interface/component.hpp>


class Move_model : public Core::Component
{
private:

  uint32_t          get_rtti() const override { return Component_type::move_model; }
  COMPONENT_RTTI(Component_type::move_model)
  
  void              on_update(const float dt) override;
  
public:

  void              strafe_left(const float left);
  void              jump();
  void              duck();
  
private:

  enum class Movement_state
  {
    grounded,
    
    jumped,
    jumping,
    landed,
    
    duck,
    ducking,
    emerged,
  };
  
  Movement_state      m_move_state      = Movement_state::grounded;
  float               m_jump_momentum   = 0.f;

  float               m_strafe          = 0.f;
  float               m_pending_strafe  = 0.1f;
  float               m_move_speed      = 5.f;
  float               m_position        = 0.f;
  float               m_cooldown        = 0.f;
  float               m_cooldown_timer  = 0.1f;
  float               m_ground          = 0.f;

}; // class


#endif // inc guard