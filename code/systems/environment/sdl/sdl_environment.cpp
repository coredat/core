#include "environment.hpp"
#include "sdl_types.hpp"
#include "sdl_common.hpp"


namespace Environment {


void
think(const Window *window,
      const Input *input,
      const std::function<void(Environment::Event_id id)> &callback)
{
  Environment::Event_id id[128]; // This will need to hold data I think.
  std::size_t current_id(0);

  // Process events
  {
    SDL_Event sdl_event;
    
    while (SDL_PollEvent(&sdl_event))
    {
      // Check for quit message.
      if (sdl_event.type == SDL_QUIT)
      {
        id[current_id++] = Environment::Event_id::window_close;
      }
    }
  }
  
  // Send out callbacks
  for(std::size_t i = 0; i < current_id; ++i)
  {
    callback(id[i]);
  }
}



} // ns