#include <core/renderer/renderer.hpp>
#include <utilities/optimizations.hpp>


namespace Core {


Renderer::Renderer(const Renderer_type renderer_type,
                   const util::generic_id arg_01,
                   const util::generic_id arg_02,
                   const util::generic_id arg_03,
                   const util::generic_id arg_04)
: m_renderer_type(renderer_type)
, m_arg_01(arg_01)
, m_arg_02(arg_02)
, m_arg_03(arg_03)
, m_arg_04(arg_04)
{
}


Renderer_type
Renderer::get_type() const
{
  return m_renderer_type;
}


const char *
Renderer::get_type_name() const
{
  switch(m_renderer_type)
  {
    case(Renderer_type::material):
      return "material renderer";
    
    case(Renderer_type::text):
      return "text renderer";
    
    default:
      return "unknown renderer";
  }
  
  UNREACHABLE;
}


util::generic_id
Renderer::get_arg_01() const
{
  return m_arg_01;
}


util::generic_id
Renderer::get_arg_02() const
{
  return m_arg_02;
}


util::generic_id
Renderer::get_arg_03() const
{
  return m_arg_03;
}


util::generic_id
Renderer::get_arg_04() const
{
  return m_arg_04;
}


} // ns