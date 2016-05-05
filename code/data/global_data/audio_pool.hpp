#ifndef AUDIO_POOL_INCLUDED_CB6DC900_EEB4_43A5_9DA6_FC45239F9135
#define AUDIO_POOL_INCLUDED_CB6DC900_EEB4_43A5_9DA6_FC45239F9135


#include <stdint.h>


#define AUDIO_SAMPLE_POOL_SIZE 128


namespace Resource_data {


struct Audio_pool
{
  uint32_t          id[AUDIO_SAMPLE_POOL_SIZE];
  //Mix_Chunk         *samples[AUDIO_SAMPLE_POOL_SIZE];
  
  const uint32_t    size = AUDIO_SAMPLE_POOL_SIZE;
};


/*!
  init.
*/
void
audio_pool_init(Audio_pool *pool);


/*!
  Add new sample
*/
void
audio_pool_add_new_sample(Audio_pool *pool, const char *filename);


/*1
  Play a sample
*/
void
audio_pool_play_sample(Audio_pool *pool);


} // ns


#endif // inc guard