#ifndef LIGHT_DATA_INCLUDED_C3947FB2_FBB6_46CA_BEE4_70EE694DF181
#define LIGHT_DATA_INCLUDED_C3947FB2_FBB6_46CA_BEE4_70EE694DF181


#include <utilities/alignment.hpp>


namespace Lighting {


// Currently this structure only represents a point light
PACK(
  struct Light
  {
    float position[3];
    float color[3];
    
    float ambient;
    float diffuse;
    float specular;

    float atten_constant;
    float atten_linear;
    float atten_exp;
  }
); // PACK


} // ns


#endif // inc guard