#ifndef UTILS_INCLUDED_0A15ED3A_0706_4C2F_98C2_F7404EA3C867
#define UTILS_INCLUDED_0A15ED3A_0706_4C2F_98C2_F7404EA3C867


#include "desc.hpp"
#include <stdint.h>


inline uint8_t
opPixelFormat8WithChannels(uint8_t channels)
{
  switch(channels)
  {
    case(1):   return opPixelFormat_R8;
    case(2):   return opPixelFormat_RG8;
    case(3):   return opPixelFormat_RGB8;
    case(4):   return opPixelFormat_RGBA8;

    default:
      return opPixelFormat_UNKNOWN;
  };
}


#endif // inc guard
