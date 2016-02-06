#include "application_common.hpp"
#include <data/core_data/core_data.hpp>
#include <systems/environment/environment.hpp>
#include <assert.h>


namespace Application {


void
common_think(
  Environment::Window *window,
  Environment::Input *input_devices,
  Core_data::Core *core_data)
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
  
  auto env_controller = input_devices->controllers[0];
  
  core_data->input_data->axis[0].x = env_controller.axis_2[1];
  core_data->input_data->axis[0].y = env_controller.axis_2[0];
  core_data->input_data->axis[1].x = env_controller.axis_1[0];
  core_data->input_data->axis[1].y = env_controller.axis_1[1];
  
  if(env_controller.buttons[0] == (uint8_t)Environment::Button_action::on_down)
  {
    core_data->input_data->buttons[0] = Core::Input::Button_state::button_down;
  }
  else
  {
    core_data->input_data->buttons[0] = Core::Input::Button_state::button_up;
  }

//  core_data->input_data->buttons[0] = (input_devices->controllers->buttons[0] == (uint8_t)Environment::Button_action::held ? Core::Input::Button_state::button_down : Core::Input::Button_state::button_up);
}


} // ns