#include <core/renderer/renderer.hpp>


namespace Core {


Renderer::Renderer(const Renderer_type renderer_type)
: m_renderer_type(renderer_type)
, m_entity_id(0)
{
}


Renderer_type
Renderer::get_type() const
{
  return m_renderer_type;
}


} // ns