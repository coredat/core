#ifndef TEXT_RENDERER_DATA_INCLUDED_6452410D_FAA8_483E_A69F_30426F4BFDB1
#define TEXT_RENDERER_DATA_INCLUDED_6452410D_FAA8_483E_A69F_30426F4BFDB1


#include <op/fwd.hpp>
#include <utilities/buffer.hpp>
#include <stdint.h>
#include <stddef.h>


namespace Data {


struct Text_renderer_data
{
  util::buffer::data font_data;
  
  util::buffer::data glyph_keys;
  util::buffer::data glyph_data;
  
  util::buffer::data string_keys;
  util::buffer::data string_data;
  util::buffer::data draw_calls;
};


} // ns


#endif // inc guard