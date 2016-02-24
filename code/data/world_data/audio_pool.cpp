#include "audio_pool.hpp"
#include <cstring>


namespace World_data {


void
audio_pool_init(Audio_pool *pool)
{
  for(uint32_t i = 0; i < pool->size; ++i)
  {
    pool->id[i] = i;
  }

  memset(&pool->samples[0], 0, sizeof(pool->samples));
}


} // ns