#ifndef ENVIRONMENT_INCLUDED_C1806561_E119_45DC_8265_4EA24894182E
#define ENVIRONMENT_INCLUDED_C1806561_E119_45DC_8265_4EA24894182E


#include "window.hpp"
#include "input.hpp"
#include "sdl/sdl_types.hpp"


namespace Environment {

enum class Event_id
{
  window_open,
  window_close,
  
  mouse_move,

  button_pressed,
  button_up,
}; // enum


void
initialize();


void
de_initialize();


void
think(const Window *window, Input *input, const std::function<void(Environment::Event_id id)> &callback);


} // ns


#endif // inc guard