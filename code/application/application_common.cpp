#include "application_common.hpp"
#include <systems/environment/environment.hpp>
#include <assert.h>


namespace Application {


void
common_think(
  Environment::Window *window,
  Environment::Input *input_devices)
{
  Environment::window_think(window);

  // Environment events.
  Environment::think(window, input_devices, [&](Environment::Event_id id)
  {
    switch (id)
    {
    case(Environment::Event_id::window_close) :
      //run_game = false;
      break;
    case(Environment::Event_id::button_pressed) :
      break;

    default:
      assert(true); // shut up the warning.
    };
  });
}


} // ns