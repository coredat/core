#ifndef TEXT_RENDERER_INCLUDED_52CB18C5_2AB3_42E7_B0DD_AEA2FF1B4394
#define TEXT_RENDERER_INCLUDED_52CB18C5_2AB3_42E7_B0DD_AEA2FF1B4394


#include "fwd.hpp"
#include <op/fwd.hpp>
#include <utilities/alloc.hpp>
#include <stdint.h>
#include <stdlib.h>


namespace Data {
namespace Text_renderer {


// ------------------------------------------------- Text Renderer Lifetime ---


Text_renderer_data *
initialize(opContext *opContext,
           opBuffer *opBuffer,
           const size_t inital_font_size_hint = 4,
           const size_t inital_glyph_size_hint = 32,
           const size_t inital_string_size_hint = 16,
           const lib::malloc_fn malloc_fn = malloc,
           const lib::realloc_fn realloc_fn = realloc,
           const lib::free_fn free_fn = free);


// --------------------------------------------------------- Font Managment ---


uint32_t
add_font(Text_renderer_data *renderer,
         const char *filename,
         opContext *ctx,
         opBuffer *buf);


// ---------------------------------------------------- Draw Call Managment ---


void
set_draw_call(Text_renderer_data *renderer,
              const uint32_t id,
              const uint32_t font_id,
              const char *str,
              const float world[16],
              opContext *ctx,
              opBuffer *buf);
  
  
void
update_draw_call_matrix(Text_renderer_data *renderer,
                        const uint32_t id,
                        const float world[16]);



void
remove_draw_call(Text_renderer_data *renderer,
                 const uint32_t id);


// ----------------------------------------------------------------- Render ---


void
render(Text_renderer_data *renderer,
       const float view_proj[16],
       opContext *ctx,
       opBuffer *buf);


} // ns
} // ns


#endif // inc guard
