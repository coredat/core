#ifndef ACTOR_CONTROLLER_INCLUDED_8C5C8BE7_A2D8_4BE1_B46E_8FC50CB81479
#define ACTOR_CONTROLLER_INCLUDED_8C5C8BE7_A2D8_4BE1_B46E_8FC50CB81479


#include <core/interface/component.hpp>
#include <core/input/controller.hpp>


class Actor_controller : public Core::Component
{
private:

  uint32_t            get_rtti() const override { return 1; };
  COMPONENT_RTTI(1)

  void                on_start() override;
  void                on_early_update(const float dt) override;
  void                on_collision(const Core::Entity &entity) override;

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

  float               m_move_speed      = 5.f;
  float               m_position        = 0.f;
  float               m_cooldown        = 0.f;
  float               m_cooldown_timer  = 0.1f;
  float               m_ground          = 0.f;

}; // class


#endif // inc guard