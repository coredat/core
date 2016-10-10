#include "sdl_message_loop.hpp"
#include <stdint.h>
#include <assert.h>
#include <3rdparty/imgui/imgui.h>
#include <3rdparty/imgui/imgui_impl_sdl_gl3.h>
#include <utilities/logging.hpp>


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
//      if(ImGui_ImplSdlGL3_ProcessEvent(&evt))
      {
//        continue;
      }
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