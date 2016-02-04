#include "../common/ids_component_types.hpp"
#include "../common/ids_game_events.hpp"
#include "local_player_controller.hpp"
#include "actor_base.hpp"
#include <core/interface/entity.hpp>
#include <core/input/input.hpp>
#include <iostream>


Local_player_controller::Local_player_controller()
{
  
}


void
Local_player_controller::on_update(const float dt)
{
  // Input
  // Movement axis
  {
    Core::Input::get_controller(Core::Input::Player::one).get_axis(0).x;
    Core::Input::get_controller(Core::Input::Player::one).get_axis(0).x;
  }
}


void
Local_player_controller::on_event(const uint32_t id,
                                  const void *data,
                                  const size_t size_of_data)
{
  switch(id)
  {
    case(Game_event_id::got_shot):
      // Get model apply damage.
      Actor_base *base = get_entity().get_component<Actor_base>(Component_type::actor);
      base->take_damage();
  }
}