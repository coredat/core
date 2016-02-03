#include "local_player_controller.hpp"
#include "../common/ids_game_events.hpp"
#include <iostream>


Local_player_controller::Local_player_controller()
{
  
}


void
Local_player_controller::on_update(const float dt)
{
  
}


void
Local_player_controller::on_event(const uint32_t id,
                                  const void *data,
                                  const size_t size_of_data)
{
  switch(id)
  {
    case(Game_event_id::got_shot):
      std::cout << "got shot" << std::endl;
  }
}