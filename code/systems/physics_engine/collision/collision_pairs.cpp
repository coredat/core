#include "collision_pairs.hpp"
#include <assert.h>


namespace Physics {
namespace Collision {


void
pairs_init(Pairs *pairs,
           const uint32_t capacity)
{
  pairs->pair_arr = new Pair[capacity];
  pairs->capacity = capacity;
  pairs->size = 0;
}


void
pairs_add(Pairs *pairs, const Pair pair)
{
  assert(pairs->size < pairs->capacity);
  
  pairs->pair_arr[pairs->size++] = pair;
}


void
pairs_free(Pairs *pairs)
{
  delete[] pairs->pair_arr;
}


} // ns
} // ns