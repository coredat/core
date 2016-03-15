#ifndef GAME_CONTROLLER_INCLUDED_3C874D05_2E91_4E3B_96BC_B2A8C771CC88
#define GAME_CONTROLLER_INCLUDED_3C874D05_2E91_4E3B_96BC_B2A8C771CC88


#include <core/interface/component.hpp>
#include "../common/ids_component_types.hpp"


class Game_controller : public Core::Component
{
private:

  uint32_t            get_rtti() const override { return Component_type::game_controller; };
  COMPONENT_RTTI(Component_type::game_controller)
  
  void                on_start() override;
  void                on_early_update(const float dt) override;

private:

  float               m_timer = 0.f;

};


#endif // inc guard