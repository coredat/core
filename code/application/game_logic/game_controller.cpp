#include "game_controller.hpp"
#include "../entity_factory.hpp"
#include <core/interface/entity.hpp>
#include "../level_functions.hpp"



void
Game_controller::on_start()
{
  Entity_factory::create_level(m_world_data);
//
//  Entity_factory::create_enemy(m_world_data);
//  Entity_factory::create_enemy(m_world_data);
//  Entity_factory::create_enemy(m_world_data);
//  Entity_factory::create_enemy(m_world_data);
//  Entity_factory::create_enemy(m_world_data);
//  Entity_factory::create_enemy(m_world_data);
//  Entity_factory::create_enemy(m_world_data);
//  Entity_factory::create_enemy(m_world_data);
//  Entity_factory::create_enemy(m_world_data);
//  Entity_factory::create_enemy(m_world_data);
//  Entity_factory::create_enemy(m_world_data);
//  Entity_factory::create_enemy(m_world_data);
}


void
Game_controller::on_early_update(const float dt)
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
}