#ifndef TEXT_RENDERER_INCLUDED_52CB18C5_2AB3_42E7_B0DD_AEA2FF1B4394
#define TEXT_RENDERER_INCLUDED_52CB18C5_2AB3_42E7_B0DD_AEA2FF1B4394


#include "fwd.hpp"
#include <stdint.h>
#include <utilities/alloc.hpp>
#include <stdlib.h>


namespace Data {


bool
initialize(Text_renderer *renderer,
           const uint32_t font_size_hint,
           const uint32_t glyph_size_hint,
           const uint32_t string_size_hint,
           const util::malloc_fn malloc_fn = malloc,
           const util::realloc_fn realloc_fn = realloc,
           const util::free_fn free_fn = free);


uint32_t
add_font(const char *filename);


void
set_draw_call(const uint32_t id,
              const char *str);


void
remove_draw_call();


void
render(const float view_proj[16]);


} // ns


#endif // inc guard