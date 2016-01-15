#ifndef SDL_TYPES_INCLUDED_EEEAFA48_6CD6_44C4_AB18_E0FE2E5AB571
#define SDL_TYPES_INCLUDED_EEEAFA48_6CD6_44C4_AB18_E0FE2E5AB571


#include "sdl_common.hpp"
#include <stdint.h>


namespace Environment {


struct Window
{
  SDL_Window *window    = nullptr;
  SDL_GLContext context = nullptr;
};


struct Input
{
  int32_t m_mouse_delta_x;
  int32_t m_mouse_delta_y;
};

  
}


#endif // inc guard