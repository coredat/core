#include "sdl_message_loop.hpp"
#include <stdint.h>
#include <assert.h>


namespace
{
  constexpr uint32_t max_sdl_callbacks(2); // 1 for window, 1 for input
  uint32_t number_of_callbacks(0);

  Sdl::Callback_event callbacks[max_sdl_callbacks];
  uintptr_t user_data[max_sdl_callbacks]; // user ptr
}


namespace Sdl {


void
event_add_callback(Callback_event cb, void *self)
{
  if(number_of_callbacks < max_sdl_callbacks)
  {
    callbacks[number_of_callbacks] = cb;
    user_data[number_of_callbacks] = (uintptr_t)self;
    
    ++number_of_callbacks;
  }
  else
  {
    assert(false); // Too many callbacks.
  }
}


void
event_process()
{
  // Process events
  SDL_Event evt;
  
  while (SDL_PollEvent(&evt))
  {
    for(uint32_t cb = 0; cb < number_of_callbacks; ++cb)
    {
      void *self = (void*)user_data[cb];
      callbacks[cb](&evt, self);
    }
  }

}


} // ns