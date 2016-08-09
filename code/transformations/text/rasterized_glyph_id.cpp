#include <transformations/text/rasterized_glyph_id.hpp>
#include <utilities/bits.hpp>


namespace Text {


uint32_t
create_glyph_id(const uint16_t font_id, const uint16_t codepoint)
{
  return util::bits_pack(font_id, codepoint);
}


} // ns