#ifndef DRAW_CALL_INCLUDED_7FD81A88_95E9_40FA_B83D_C41F439569DE
#define DRAW_CALL_INCLUDED_7FD81A88_95E9_40FA_B83D_C41F439569DE


#include <op/fwd.hpp>


namespace Data {


struct Draw_call
{
  float     world_matrix[16];
  opID      string_glyphs;
  opID      glyph_lookup_texture;
  opID      glyph_texture;
};


} // ns


#endif // inc guard