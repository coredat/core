#include "game_controller.hpp"
#include "../entity_factory.hpp"
#include <core/interface/entity.hpp>
#include "../level_functions.hpp"
#include "../common/ids_game_events.hpp"



void
Game_controller::on_start()
{
  Entity_factory::create_level(m_world_data);
  
  get_entity().send_event(Game_event_id::game_state_initial_load, nullptr, 0);
  get_entity().send_event(Game_event_id::game_state_playing, nullptr, 0);
}


void
Game_controller::on_early_update(const float dt)
{
}