#include "audio_pool.hpp"
#include <cstring>


namespace Resource_data {


void
audio_pool_init(Audio_pool *pool)
{
  for(uint32_t i = 0; i < pool->size; ++i)
  {
    pool->id[i] = i;
  }
  
  memset(&pool->samples[0], 0, sizeof(pool->samples));
  
  const int32_t flags = 0;
  const int32_t initted = Mix_Init(flags);

  if((initted & flags) != flags)
  {
    printf("Mix_Init: Failed to init required ogg and mod support!\n");
    printf("Mix_Init: %s\n", Mix_GetError());
  }
  
  if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024)==-1)
  {
    printf("Mix_OpenAudio: %s\n", Mix_GetError());
  }
  
  const uint32_t num_chans = 256;
  
  Mix_AllocateChannels(num_chans);
//  channels.resize(num_chans);
  
  // Make all channels true
//  for(uint32_t i = 0; i < channels.size(); ++i)
//  {
//    channels[i] = true;
//  }
  
//  Mix_ChannelFinished(&channel_finished);
  
}


void
audio_pool_add_new_sample(Audio_pool *pool, const char *filename)
{
  Mix_Chunk *load_chunk = Mix_LoadWAV(filename);
  
  pool->samples[0] = load_chunk;
}


void
audio_pool_play_sample(Audio_pool *pool)
{
  if(Mix_PlayChannel(1, pool->samples[0], 0))
  {
    printf("Mix_PlayChannel: %s\n",Mix_GetError());
  }
}


} // ns