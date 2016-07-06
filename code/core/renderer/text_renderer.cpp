#include <core/renderer/text_renderer.hpp>
#include <core/renderer/renderer.hpp>
#include <core/font/font.hpp>


namespace Core {


Text_renderer::Text_renderer()
{
}


void
Text_renderer::set_font(const Font &font)
{
}


Font
Text_renderer::get_font() const
{
  return Font("");
}


void
Text_renderer::set_text(const char *str)
{
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


} // ns