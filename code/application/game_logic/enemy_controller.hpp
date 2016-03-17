#ifndef ENEMY_CONTROLLER_INCLUDED_FA81C106_3E65_4175_9CDC_AE26EDFE1D99
#define ENEMY_CONTROLLER_INCLUDED_FA81C106_3E65_4175_9CDC_AE26EDFE1D99


#include <core/interface/component.hpp>
#include "../common/ids_component_types.hpp"


class Enemy_controller : public Core::Component
{
private:

  uint32_t            get_rtti() const override { return Component_type::enemy_controller; };
  COMPONENT_RTTI(Component_type::enemy_controller)

  void                on_start() override;
  void                on_end() override;
  void                on_early_update(const float dt) override;
  void                on_collision(const Core::Entity &collided_with) override;

private:
  
  enum class State
  {
    up_down_search,
  };
  
  State               m_state = State::up_down_search;
  
  int32_t             m_direction = 1;
  uint32_t            m_climb_speed = 7;

}; // class


#endif // inc guard