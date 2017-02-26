#include <core/renderer/text_renderer.hpp>
#include <core/renderer/renderer.hpp>
#include <core/font/font.hpp>
#include <common/fixed_string_search.hpp>
#include <data/context/text_mesh_data.hpp>
#include <data/context_data.hpp>
#include <systems/text/character.hpp>

#include <graphics_api/utils/geometry.hpp>
#include <graphics_api/mesh.hpp>
#include <graphics_api/vertex_format.hpp>

#include <transformations/text/rasterized_glyph_id.hpp>

#include <utilities/assert.hpp>


namespace Core {


struct Text_renderer::Impl
{
  std::string       text;
  uint32_t  font_id;
};


Text_renderer::Text_renderer()
: m_impl(new Impl{"", 0})
{
  LIB_ASSERT(m_impl);
}


Text_renderer::Text_renderer(const Font &font,
                             const char *str)
: Text_renderer()
{
  LIB_ASSERT(m_impl);
  
  set_font(font);
  set_text(str);
}


Text_renderer::~Text_renderer()
{
  LIB_ASSERT(m_impl);
  
  delete m_impl;
}


Text_renderer::Text_renderer(const Text_renderer &other)
: m_impl(new Impl{"", 0})
{
  LIB_ASSERT(m_impl);
  
  set_font(other.get_font());
  set_text(other.get_text());
}


Text_renderer::Text_renderer(Text_renderer &&other)
: m_impl(new Impl{"", 0})
{
  LIB_ASSERT(m_impl);
  
  set_font(other.get_font());
  set_text(other.get_text());
  
  other.set_font(Font());
  other.set_text("");
}


Text_renderer&
Text_renderer::operator=(const Text_renderer &other)
{
  LIB_ASSERT(m_impl);
  
  set_font(other.get_font());
  set_text(other.get_text());
  
  return *this;
}


Text_renderer&
Text_renderer::operator=(Text_renderer &&other)
{
  LIB_ASSERT(m_impl);
  
  set_font(other.get_font());
  set_text(other.get_text());
  
  other.set_font(Font());
  other.set_text("");
  
  return *this;
}


void
Text_renderer::set_font(const Font &font)
{
  LIB_ASSERT(m_impl);
  
  m_impl->font_id = font.get_id();
}


Font
Text_renderer::get_font() const
{
  LIB_ASSERT(m_impl);
  
  return Font(m_impl->font_id);
}


void
Text_renderer::set_text(const char *str)
{
  LIB_ASSERT(m_impl);
  
  m_impl->text = std::string(str);
}


const char*
Text_renderer::get_text() const
{
  LIB_ASSERT(m_impl);
  
  return m_impl->text.c_str();
}


void
Text_renderer::set_text_size(const uint32_t size)
{
  LIB_ASSERT(m_impl);
  
  LOG_TODO("Not supported yet.")
  
  LIB_ASSERT(false);
}


uint32_t
Text_renderer::get_text_size() const
{
  LIB_ASSERT(m_impl);
  
  LOG_TODO("Not supported yet.")
  
  LIB_ASSERT(false);
  
  return 0;
}


Text_renderer::operator Renderer() const
{
//  return Renderer(Core::Renderer_type::text, m_font_id, m_text_id);
  LOG_DEPRECATED("This is no longer the way todo it.")
  return Renderer();
}


} // ns
