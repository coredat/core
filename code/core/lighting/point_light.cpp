#include <core/lighting/point_light.hpp>


namespace Core {


Point_light::Point_light()
: Point_light(Core::Color(0xFFFFFFFF),
              0.0f,     // amb
              1.0f,     // spec
              1.0f,     // diffuse
              1.0f,     // const
              0.027f,   // linear
              0.0028f)  // exp
{
}


Point_light::Point_light(const Core::Color &color,
                         const float ambient,
                         const float specular,
                         const float diffuse,
                         const float constant,
                         const float linear,
                         const float exponential)
: m_color(color)
, m_ambient(ambient)
, m_specular(specular)
, m_diffuse(diffuse)
, m_constant(constant)
, m_linear(linear)
, m_exponential(exponential)
{
}


void
Point_light::set_color(const Core::Color &color)
{
  m_color = color;
}


Core::Color
Point_light::get_color() const
{
  return m_color;
}


void
Point_light::set_ambient(const float amb)
{
  m_ambient = amb;
}


float
Point_light::get_ambient() const
{
  return m_ambient;
}


void
Point_light::set_diffuse(const float diff)
{
  m_diffuse = diff;
}


float
Point_light::get_diffuse() const
{
  return m_diffuse;
}


void
Point_light::set_specular(const float spec)
{
  m_specular = spec;
}


float
Point_light::get_specular() const
{
  return m_specular;
}


void
Point_light::set_attenuation(const float constant,
                             const float linear,
                             const float exponential)
{
  m_constant = constant;
  m_linear = linear;
  m_exponential = exponential;
}


float
Point_light::get_constant_attenuation() const
{
  return m_constant;
}


float
Point_light::get_linear_attenuation() const
{
  return m_linear;
}


float
Point_light::get_exponential_attenuation() const
{
  return m_exponential;
}


} // ns