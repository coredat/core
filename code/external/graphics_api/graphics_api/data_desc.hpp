#ifndef DATA_DESC_INCLUDED_9259FDA0_760D_4435_9C58_46FBCFE8B358
#define DATA_DESC_INCLUDED_9259FDA0_760D_4435_9C58_46FBCFE8B358


#include "gfx_common.hpp"


namespace Graphics_api {


namespace Data_type {
enum ENUM : uint8_t {
  
  unknown,
  
  unknown_data,
  unknown_sampler,

};
} // ns


struct Data_desc
{
  uint8_t         data_type       = Data_type::unknown;
  
  // Handles
  uintptr_t       platform_handle = 0;
  uintptr_t       internal_handle = 0;
};


// Platform thing
struct Uniform
{
  const char name[64];
  
};


} // ns


#endif // inc guard