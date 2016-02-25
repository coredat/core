#include "game_state.hpp"
#include <application/common/ids_game_events.hpp>


void
Game_state::on_start()
{
}


void
Game_state::on_event(const uint32_t event_id, const void *data, const uint32_t size_of_data)
{
  if(event_id == Game_event_id::guards_sounded_alarm)
  {
    // Start the count down.
  }
}