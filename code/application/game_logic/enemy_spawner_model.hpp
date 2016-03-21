#ifndef ENEMY_SPAWNER_INCLUDED_BF7244E9_24DD_4224_BB06_7096F1AB9B82
#define ENEMY_SPAWNER_INCLUDED_BF7244E9_24DD_4224_BB06_7096F1AB9B82


#include <core/interface/component.hpp>
#include "../common/ids_component_types.hpp"


class Enemy_spawner_model : public Core::Component
{
private:

  uint32_t            get_rtti() const override { return Component_type::enemy_spawner_model; };
  COMPONENT_RTTI(Component_type::enemy_spawner_model)
  
  void                on_update(const float dt) override;
  void                on_event(const uint32_t event_id,
                               const void *data,
                               const uint32_t size_of_data) override;

private:

  enum class State
  {
    no_spawning,
    spawning,
  };
  
  State               m_state = State::no_spawning;
  float               m_timer = 0.f;
  float               m_spawn_timer = 0.4f;

}; // class


#endif // inc guard