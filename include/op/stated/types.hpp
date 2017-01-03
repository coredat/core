#ifndef TYPES_INCLUDED_2C459E86_695B_4F81_8A5C_E0A32190AE84
#define TYPES_INCLUDED_2C459E86_695B_4F81_8A5C_E0A32190AE84


#include "../common/common.hpp"


namespace op {
namespace type {


enum ENUM : uint8_t
{
  NONE,

  FLOAT,
  FLOAT2,
  FLOAT3,
  FLOAT4,

  INT,
  INT2,
  INT3,
  INT4,

  MAT3,
  MAT4,
  TEXTURE_1D,
  TEXTURE_2D,
  TEXTURE_3D,
};


constexpr uint8_t
element_count(const uint8_t element)
{
  switch(element)
  {
    case(FLOAT):     return 1;
    case(FLOAT2):    return 2;
    case(FLOAT3):    return 3;
    case(FLOAT4):    return 4;

    case(INT):       return 1;
    case(INT2):      return 2;
    case(INT3):      return 3;
    case(INT4):      return 4;

    default:         return 0;
  }
}


} // ns
} // ns


#endif // inc guard
