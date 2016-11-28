#ifndef ENVELOPE_INCLUDED_3161EA44_4342_467C_97C9_2023BCD460CE
#define ENVELOPE_INCLUDED_3161EA44_4342_467C_97C9_2023BCD460CE


#include <stdint.h>


namespace Graphics_api {


struct Envelope
{
  uint32_t x_from = 0;
  uint32_t x_to = 0;
  
  uint32_t y_from = 0;
  uint32_t y_to = 0;
  
  uint32_t z_from = 0;
  uint32_t z_to = 0;
};


} // ns


#endif // inc guard