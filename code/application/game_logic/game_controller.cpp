#include "game_controller.hpp"
#include "../entity_factory.hpp"
#include <core/interface/entity.hpp>
#include "../level_functions.hpp"



void
Game_controller::on_start()
{
  Entity_factory::create_enemy(m_world_data);
  Entity_factory::create_enemy(m_world_data);
  Entity_factory::create_enemy(m_world_data);
  Entity_factory::create_enemy(m_world_data);
  Entity_factory::create_enemy(m_world_data);
  Entity_factory::create_enemy(m_world_data);
  Entity_factory::create_enemy(m_world_data);
  Entity_factory::create_enemy(m_world_data);
  Entity_factory::create_enemy(m_world_data);
  Entity_factory::create_enemy(m_world_data);
  Entity_factory::create_enemy(m_world_data);
  Entity_factory::create_enemy(m_world_data);
}


void
Game_controller::on_early_update(const float dt)
{
  m_timer += dt;
  
  if(m_timer > 3.f)
  {
    Entity_factory::create_enemy(m_world_data);
    
    m_timer = 0;
  }
}