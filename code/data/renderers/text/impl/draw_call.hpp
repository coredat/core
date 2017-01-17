#ifndef DRAW_CALL_INCLUDED_7FD81A88_95E9_40FA_B83D_C41F439569DE
#define DRAW_CALL_INCLUDED_7FD81A88_95E9_40FA_B83D_C41F439569DE


#include <op/fwd.hpp>


namespace Data {


struct Draw_call
{
  float               world_matrix[16];
  float               wvp_matrix[16]; // This is a hack while op buffer doesn't have memory support.
  opID                texture;
  opID                glyph_metrics;
  opID                string_info;
  uint32_t            string_size;
  uint32_t            cull_mask = -1;
};


} // ns


#endif // inc guard