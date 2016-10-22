#include "sdl_message_loop.hpp"
#include <stdint.h>
#include <assert.h>
#include <utilities/logging.hpp>

//#define NK_SDL_GL3_IMPLEMENTATION
#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_API

#include <3rdparty/nuklear/nuklear.h>
#include <3rdparty/nuklear/nuklear_sdl_gl3.h>


namespace
{
  constexpr uint32_t max_sdl_callbacks(2); // 1 for window, 1 for input

  Sdl::Callback_event callbacks[max_sdl_callbacks] = {nullptr, nullptr};
  uintptr_t user_data[max_sdl_callbacks] = {0, 0};
}


namespace Sdl {


void
event_add_callback(Callback_event cb, int slot, void *self)
{
  LOG_TODO_ONCE("SDL subscription callback's are dreadful!");

  if(slot < max_sdl_callbacks)
  {
    callbacks[slot] = cb;
    user_data[slot] = (uintptr_t)self;
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
    for(uint32_t cb = 0; cb < max_sdl_callbacks; ++cb)
    {
      #ifdef CORE_DEBUG_MENU
      nk_sdl_handle_event(&evt);
      #endif
      
      if(callbacks[cb])
      {
        void *self = (void*)user_data[cb];
        callbacks[cb](&evt, self);
      }
    }
  }
}


} // ns