#include <core/renderer/text_renderer.hpp>
#include <core/renderer/renderer.hpp>
#include <core/font/font.hpp>
#include <data/global_data/text_mesh_data.hpp>
#include <data/global_data/resource_data.hpp>
#include <systems/text/character.hpp>

#include <graphics_api/utils/geometry.hpp>
#include <graphics_api/mesh.hpp>
#include <graphics_api/vertex_format.hpp>

#include <transformations/text/rasterized_glyph_id.hpp>


namespace Core {


Text_renderer::Text_renderer()
: m_font_id(util::generic_id_invalid())
, m_text_id(util::generic_id_invalid())
{
}


Text_renderer::Text_renderer(const util::generic_id font_id,
                             const util::generic_id text_id)
: m_font_id(font_id)
, m_text_id(text_id)
{
  LOG_TODO("Check these ids are valid.");
}


void
Text_renderer::set_font(const Font &font)
{
  m_font_id = font.get_id();
  
  // Set the font id for this item,
  // but only if it exists.
  if(m_text_id)
  {
    auto resources = Resource_data::get_resources();
    assert(resources);
    
    auto text_mesh_data = resources->text_mesh_data;
    assert(text_mesh_data);
  
    Resource_data::data_lock(text_mesh_data);
    Resource_data::text_mesh_data_set_property_font_id(text_mesh_data, m_text_id, m_font_id);
    Resource_data::data_unlock(text_mesh_data);
  }
}


Font
Text_renderer::get_font() const
{
  assert(false);
  return Font("");
}


void
Text_renderer::set_text(const char *str)
{
  auto resources = Resource_data::get_resources();
  assert(resources);
  
  auto text_mesh_data = resources->text_mesh_data;
  assert(text_mesh_data);

  // Set the text resource
  // But don't build the mesh
  {
    Resource_data::data_lock(text_mesh_data);
    
    // Search for text, if it doesn't exist then add it.
    if(!Resource_data::text_mesh_data_search_property_text(text_mesh_data, str))
    {
      m_text_id = Resource_data::text_mesh_data_push_back(text_mesh_data);
    }
    
    Resource_data::text_mesh_data_set_property_text(text_mesh_data, m_text_id, str);
    Resource_data::text_mesh_data_set_property_font_id(text_mesh_data, m_text_id, m_font_id);
    
    Resource_data::data_unlock(text_mesh_data);
  }
}


const char*
Text_renderer::get_text() const
{
  return "";
}


Text_renderer::operator Renderer() const
{
  return Renderer(Core::Renderer_type::text, m_font_id, m_text_id);
}


util::generic_id
Text_renderer::get_font_id() const
{
  return m_font_id;
}


util::generic_id
Text_renderer::get_text_id() const
{
  return m_text_id;
}


} // ns