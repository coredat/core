#ifndef TICK_INFORMATION_INCLUDED_E674BDA9_2240_4B8B_97F1_CEE0810FA0B6
#define TICK_INFORMATION_INCLUDED_E674BDA9_2240_4B8B_97F1_CEE0810FA0B6


#include <stdint.h>


namespace Engine {

struct Tick_information
{


  // Renderer things
  uint32_t number_of_draw_calls = 0;
  uint32_t camera_runs = 0;
};

} // ns


#endif // inc guard