#ifndef TEXT_RENDERER_DATA_INCLUDED_6452410D_FAA8_483E_A69F_30426F4BFDB1
#define TEXT_RENDERER_DATA_INCLUDED_6452410D_FAA8_483E_A69F_30426F4BFDB1


#include "text_renderer_fwd.hpp"
#include <op/fwd.hpp>
#include <utilities/buffer.hpp>
#include <stdint.h>
#include <stddef.h>


namespace Data {


struct Text_renderer
{
  uint32_t    *font_keys_array  = nullptr;
  
  
  uint32_t    *glyph_keys_array = nullptr;
  uint32_t    *glyphs_array     = nullptr;
  
  uint32_t    *draw_keys_array  = nullptr;
  
  const char  *strings_array    = nullptr;
  Draw_call   *draw_call_arr    = nullptr;
  
  size_t      data_size         = 0;
  size_t      data_capacity     = 0;
};


} // ns


#endif // inc guard