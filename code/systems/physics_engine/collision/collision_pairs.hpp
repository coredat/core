#ifndef COLLISION_PAIRS_INCLUDED_7B65C253_C5A3_4C80_94B9_7ED02B7B85AF
#define COLLISION_PAIRS_INCLUDED_7B65C253_C5A3_4C80_94B9_7ED02B7B85AF


#include <stdint.h>


namespace Physics {
namespace Collision {


struct Pair
{
  uint32_t a;
  uint32_t b;
};


struct Pairs
{
  Pair      *pair_arr;
  uint32_t  size = 0;
  uint32_t  capacity = 0;
};


void
pairs_init(Pairs *pairs, const uint32_t capacity);


void
pairs_add(Pairs *pairs, const Pair pair);


void
pairs_free(Pairs *pairs);


} // ns
} // ns


#endif // inc guard