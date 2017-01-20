#include <core/font/font.hpp>
#include <transformations/font/font_resource.hpp>
#include <data/context_data.hpp>
#include <data/memory/memory_data.hpp>
#include <data/context/font_data.hpp>
#include <data/context/texture_data.hpp>
#include <data/renderers/text/text_renderer.hpp>
#include <core/world/detail/world_index.hpp>
#include <common/fixed_string_search.hpp>
#include <utilities/string_helpers.hpp>
#include <3rdparty/stb/stb_truetype.h>
#include <stdio.h>
#include <stdlib.h>


namespace Core {


Font::Font()
: m_font_id(0)
{
}


Font::Font(const char *filename)
: Font::Font()
{
  /*
    Check to see if the font name exists.
    If it does we just use that id.
    else we create and add a new one.
  */
  auto resources = Data::get_context_data();
  assert(resources);
  
  auto font_data = resources->font_data;
  assert(font_data);
  
  auto texture_data = resources->texture_data;
  assert(texture_data);
  
  Data::data_lock(font_data);
  Data::data_lock(texture_data);

  char font_name[256];
  memset(font_name, 0, sizeof(font_name));
  util::filename_from_path(filename, font_name);
  
  size_t search_id = 0;
  if(Common::fixed_string_search(font_name,
                                 Data::font_get_name_data(font_data),
                                 Data::font_get_name_stride(),
                                 Data::font_get_size(font_data),
                                 &search_id))
  {
    m_font_id = font_data->keys[search_id];
  }
  
  Data::data_unlock(texture_data);
  Data::data_unlock(font_data);

  if(!m_font_id)
  {
    m_font_id = Font_resource::add_new_font(filename, font_data, texture_data, resources->op_context, resources->op_buffer);
  }
  
  // -- New Text Renderer -- //
  // -- We are overriding the other font id -- //
  std::shared_ptr<Data::World> world = Core_detail::world_index_get_world_data(1);
  assert(world);
  
  m_font_id = Data::Text_renderer::add_font(
    world->text_renderer,
    filename,
    resources->op_context,
    resources->op_buffer
  );
}


Font::Font(const util::generic_id id)
{
  m_font_id = id;
}


util::generic_id
Font::get_id() const
{
  return m_font_id;
}


} // ns