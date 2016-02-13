#ifndef INTERPOLATION_POOL_INCLUDED_D00F680C_1C8C_4E03_B60C_734D1DFA5088
#define INTERPOLATION_POOL_INCLUDED_D00F680C_1C8C_4E03_B60C_734D1DFA5088


#include "net_entity_pool.hpp"
#include <stdint.h>


#define NUMBER_OF_SNAPSHOTS 4


namespace Net_data {


struct Interpolation_pool
{
  Net_entity_pool snapshot[NUMBER_OF_SNAPSHOTS];
  uint32_t        last_tick     = 0;
  uint32_t        rotate_point  = 0;
  const uint32_t  max_snapshots = NUMBER_OF_SNAPSHOTS;
};


} // ns


#endif // inc guard