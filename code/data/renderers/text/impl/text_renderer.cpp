#include "../text_renderer.hpp"
#include "text_renderer_data.hpp"
#include <utilities/assert.hpp>
#include <utilities/logging.hpp>
#include <op/op.hpp>


namespace {


opID text_renderer_shader = 0;


} // ns


namespace Data {


bool
initialize(Text_renderer *renderer,
           const uint32_t font_size_hint,
           const uint32_t glyph_size_hint,
           const uint32_t string_size_hint)
{
  // -- Param Check -- //
  ASSERT(renderer);
  ASSERT(glyph_size_hint);
  ASSERT(font_size_hint);
  ASSERT(string_size_hint);

  if(renderer->data_capacity == 0)
  {
    LOG_ERROR("Renderer has alreay been initialized");
    return false;
  }
  
  // -- Initialize shader -- //
  if(text_renderer_shader == 0)
  {
    
  }
  
  // -- Initialize data stores -- //
  {
    renderer->font_keys_array =
  }
  
  // -- Finished Setup -- //
  return true;
}


uint32_t
add_font(const char *filename)
{
  
}


void
set_draw_call(const uint32_t id,
              const char *str)
{
}


void
remove_draw_call()
{
}


void
render(const float view_proj[16])
{
}


} // ns