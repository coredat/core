#ifndef TEXTURE_DESC_INCLUDED_7A7FCB87_9AB6_45E3_8A60_E11D298058C1
#define TEXTURE_DESC_INCLUDED_7A7FCB87_9AB6_45E3_8A60_E11D298058C1


#include "pixel_format.hpp"
#include "dimention.hpp"
#include "texture_filtering.hpp"
#include <stdint.h>


namespace Graphics_api {


struct Texture_desc
{
  Pixel_format      pixel_format    = Pixel_format::rgba8;
  Dimention         dimention       = Dimention::two;
  uint32_t          width           = 0;
  uint32_t          height          = 0;
  uint32_t          depth           = 0;
  bool              mips            = false;
  Texture_filtering filtering       = filter_create(Filtering_mode::bilinear, Wrap_mode::clamp);
  uintptr_t         platform_handle = 0;
  uintptr_t         internal_handle = 0;
};


} // ns


#endif // inc guard