#include "audio_pool.hpp"


namespace World_data {


void
audio_pool_init(Audio_pool *pool)
{
  for(size_t i = 0; i < pool->size; ++i)
  {
    pool->id[i] = i;
  }

  memset(&pool->samples[0], 0, sizeof(pool->samples));
}


} // ns