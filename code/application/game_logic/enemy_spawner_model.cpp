#include "enemy_spawner_model.hpp"
#include "../entity_factory.hpp"
#include <core/interface/entity.hpp>
#include "../common/ids_game_events.hpp"


void
Enemy_spawner_model::on_update(const float dt)
{

  switch(m_state)
  {
    case(State::spawning):
    {
      m_timer += dt;
      
      if(m_timer > m_spawn_timer)
      {
        // In case of frame dip we can spawn multiple enemies.
        const uint32_t number_to_spawn = static_cast<uint32_t>(std::ceil(m_timer / m_spawn_timer));
      
        for(uint32_t i = 0; i < number_to_spawn; ++i)
        {
          Entity_factory::create_enemy(m_world_data);
        }
        
        m_timer = 0;
      }
    
      break;
    }
    
    default:
      break;
  }
}


void
Enemy_spawner_model::on_event(const uint32_t event_id,
                              const void *data,
                              const uint32_t size_of_data)
{
  if(event_id == Game_event_id::game_state_playing)
  {
    m_state = State::spawning;
  }
}