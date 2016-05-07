#include "collision_pairs.hpp"
#include <assert.h>

#define CORE_USE_SCRATCH_ALLOC

#ifdef CORE_USE_SCRATCH_ALLOC
#include <data/global_data/memory_data.hpp>
#include <new>
#endif


namespace Physics {
namespace Collision {


void
pairs_init(Pairs *pairs,
           const uint32_t capacity)
{
  #ifdef CORE_USE_SCRATCH_ALLOC
  const size_t bytes = sizeof(Pair) * capacity;

  pairs->pair_arr = new(memory::scratch_alloc(bytes)) Pair[capacity];
  #else
  pairs->pair_arr = new Pair[capacity];
  #endif

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
  #ifndef CORE_USE_SCRATCH_ALLOC
  delete[] pairs->pair_arr;
  #endif
}


} // ns
} // ns