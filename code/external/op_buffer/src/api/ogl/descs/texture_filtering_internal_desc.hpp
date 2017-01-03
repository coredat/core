#ifdef OP_BUFFER_API_OGL
#ifndef TEXTURE_FILTERING_INTERNAL_DESC_INCLUDED_DD28D9F8_661D_4F97_AC5A_D12C02668A03
#define TEXTURE_FILTERING_INTERNAL_DESC_INCLUDED_DD28D9F8_661D_4F97_AC5A_D12C02668A03


#include "../details/ogl_common.hpp"


namespace op {
namespace ogl {


struct texture_filtering_internal_desc
{
  // We can't bake the filtering until the texture is bound.
  // The presence of mips or not changes how we would deal with

  // So we will just mimic the public desc for this one.
  uint8_t filter_mode;
  uint8_t filter_multiplier;

  uint8_t wrap_mode_width;
  uint8_t wrap_mode_height;
  uint8_t wrap_mode_depth;
};


} // ns
} // ns


#endif // inc guard
#endif // api guard
