#ifndef POINT_LIGHT_INCLUDED_652131E2_7D4F_4ADA_B3DF_551F8216666A
#define POINT_LIGHT_INCLUDED_652131E2_7D4F_4ADA_B3DF_551F8216666A


#include <core/common/core_fwd.hpp>


namespace Core {


class Point_light final
{
public:
  
  explicit        Point_light();
                  Point_light(const Core::Color &light_color,
                              const float ambient,
                              const float specular,
                              const float diffuse,
                              const float constant,
                              const float linear,
                              const float exponential);
  
  void            set_color(const Core::Color &color);
  Core::Color     get_color() const;
  
  void            set_ambient(const float amb);
  float           get_ambient() const;
  
  void            set_diffuse(const float diff);
  float           get_diffuse() const;
  
  void            set_specular(const float spec);
  float           get_specular() const;
  
  void            set_attenuation(const float constant,
                                  const float linear,
                                  const float exponential);
  float           get_constant_attenuation() const;
  float           get_linear_attenuation() const;
  float           get_exponential_attenuation() const;
  
private:

  uint32_t        m_color;
  float           m_ambient;
  float           m_specular;
  float           m_diffuse;
  float           m_constant;
  float           m_linear;
  float           m_exponential;

};


} // ns


#endif // inc guard