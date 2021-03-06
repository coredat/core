#include <core/renderer/renderer.hpp>
#include <core/renderer/material_renderer.hpp>
#include <core/renderer/text_renderer.hpp>
#include <common/error_strings.hpp>
#include <utilities/logging.hpp>
#include <utilities/optimizations.hpp>


namespace Core {


Renderer::Renderer()
: Renderer(0)
{
}


Renderer::Renderer(uint32_t id)
{
}


//Renderer::Renderer()
//: Renderer(Renderer_type::unknown)
//{
//}
//
//
//Renderer::Renderer(const uint32_t renderer_type,
//                   const uint32_t arg_01,
//                   const uint32_t arg_02,
//                   const uint32_t arg_03)
//: m_renderer_type(renderer_type)
//, m_arg_01(arg_01)
//, m_arg_02(arg_02)
//, m_arg_03(arg_03)
//{
//}
//
//
////uint32_t
////Renderer::get_type() const
////{
////  return m_renderer_type;
////}
//
//
//Renderer&
//Renderer::operator=(const Renderer &other)
//{
//  m_renderer_type = other.m_renderer_type;
//  m_arg_01        = other.m_arg_01;
//  m_arg_02        = other.m_arg_02;
//  m_arg_03        = other.m_arg_03;
//  
//  return *this;
//}
//
//
//const char *
//Renderer::get_type_name() const
//{
//  switch(m_renderer_type)
//  {
//    case(Renderer_type::material):
//      return "material renderer";
//    
//    case(Renderer_type::text):
//      return "text renderer";
//    
//    default:
//      return "unknown renderer";
//  }
//  
//  UNREACHABLE;
//}
//
//
//uint32_t
//Renderer::get_arg_01() const
//{
//  return m_arg_01;
//}
//
//
//uint32_t
//Renderer::get_arg_02() const
//{
//  return m_arg_02;
//}
//
//
//uint32_t
//Renderer::get_arg_03() const
//{
//  return m_arg_03;
//}
//
//
//Renderer::operator bool() const
//{
//  return !(m_renderer_type == Renderer_type::unknown);
//}
//
//
//Renderer::operator Material_renderer() const
//{
//  if(m_renderer_type != Renderer_type::material)
//  {
//    LOG_ERROR(Error_string::invalid_cast());
//  }
//  
//  return Renderer_utils::cast_to_material_renderer(*this);
//}
//
//
//Renderer::operator Text_renderer() const
//{
//  if(m_renderer_type != Renderer_type::text)
//  {
//    LOG_ERROR(Error_string::invalid_cast());
//  }
//  
//  return Renderer_utils::cast_to_text_renderer(*this);
//}


} // ns
