#ifndef COLOR_INCLUDED_FA0621D5_83C0_4765_8B7C_E44F07C3EC2F
#define COLOR_INCLUDED_FA0621D5_83C0_4765_8B7C_E44F07C3EC2F


#include <stdint.h>


namespace Core {


class Color
{
public:

  explicit      Color(const uint32_t hex_value = 0xFFFFFFFF);
  
private:

  uint32_t      m_color;

};


} // ns


#endif // inc guard