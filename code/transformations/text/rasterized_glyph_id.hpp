#ifndef RASTERIZED_GLYPH_ID_INCLUDED_BE23491D_3FBA_481C_B5A5_ECD4534BF858
#define RASTERIZED_GLYPH_ID_INCLUDED_BE23491D_3FBA_481C_B5A5_ECD4534BF858


#include <stdint.h>


namespace Text {


/*!
  Creates a glyph_id for use in the data pool.
*/
uint32_t
create_glyph_id(const uint16_t font_id, const uint16_t codepoint);


} // ns


#endif // inc guard