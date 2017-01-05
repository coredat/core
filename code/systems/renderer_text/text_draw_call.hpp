#ifndef DRAW_CALL_INCLUDED_A021FB15_A654_4DDF_8032_6E52F9FFCF85
#define DRAW_CALL_INCLUDED_A021FB15_A654_4DDF_8032_6E52F9FFCF85


#include <stdint.h>
#include <op/fwd.hpp>


namespace Text_renderer {


struct Draw_call
{
  float               world_matrix[16];
  opID                texture;
  opID                glyph_metrics;
  opID                string_info;
  uint32_t            string_size;
  uint32_t            cull_mask = -1;
};

} // ns


#endif // inc guard