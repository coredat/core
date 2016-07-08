#ifndef TARGET_INCLUDED_AB4C3931_400F_48C6_A87E_BF9418EB8201
#define TARGET_INCLUDED_AB4C3931_400F_48C6_A87E_BF9418EB8201


#include "pixel_format.hpp"


namespace Graphics_api {


struct Target
{
};


void
target_create(Target *target, const Pixel_format color);


void
target_bind(const Target *target);


} // ns


#endif // inc guard