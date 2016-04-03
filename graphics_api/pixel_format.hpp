#ifndef PIXEL_FORMAT_INCLUDED_A26C33CD_4B44_4B8C_B71D_4F87D12AE653
#define PIXEL_FORMAT_INCLUDED_A26C33CD_4B44_4B8C_B71D_4F87D12AE653


#include <stdint.h>


namespace Graphics_api {


enum class Pixel_format : uint8_t
{
  rgba8,
  rgba32f,
  
  unknown,
};


} // ns


#endif // inc guard