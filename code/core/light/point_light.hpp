#ifndef POINT_LIGHT_INCLUDED_652131E2_7D4F_4ADA_B3DF_551F8216666A
#define POINT_LIGHT_INCLUDED_652131E2_7D4F_4ADA_B3DF_551F8216666A


#include <core/common/core_fwd.hpp>


namespace Core {


class Point_light final
{
public:
  
  explicit        Point_light();
  explicit        Point_light(Core::World &world);
  
  void            set_color(const Core::Color &color);
  
  void            set_ambient(const float amb);
  void            set_diffuse(const float diff);
  void            set_attenuation(const float constant,
                                  const float linear,
                                  const float exponential);
  


private:


};


} // ns


#endif // inc guard