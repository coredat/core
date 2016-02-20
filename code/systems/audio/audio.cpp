#include "audio.hpp"
#include <iostream>
#include <algorithm>
#include <vector>


namespace
{
  std::vector<bool> channels;
  
  
  void
  channel_finished(int channel)
  {
    channels[channel] = true;
  }
  
  
  inline int
  get_free_channel()
  {
    for(size_t i = 0; i < channels.size(); ++i)
    {
      if(channels[i])
      {
        channels[i] = false;
        return i;
      }
    }
    
    return -1;
  }
}


namespace Audio {


void
initialize()
{
#ifndef WIN32
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
  channels.resize(num_chans);
  
  // Make all channels true
  for(size_t i = 0; i < channels.size(); ++i)
  {
    channels[i] = true;
  }
  
  Mix_ChannelFinished(&channel_finished);
#endif
}


void
de_initialize()
{
#ifndef WIN32
  // Because multiple init's are allowed with SDL_Mixer we need
  // to repeated De-init until its done.
  while(Mix_Init(0))
  {
    Mix_Quit();
  }
#endif
}


void
load_samples(const char* files_to_load[],
             const size_t number_of_files,
             Sample out_samples[],
             const size_t number_of_out_samples)
{
#ifndef WIN32
  const size_t number_to_load = std::min(number_of_files, number_of_out_samples);

  for(size_t i = 0; i < number_to_load; ++i)
  {
    Mix_Chunk *load_chunk = Mix_LoadWAV(files_to_load[i]);
    
    if(!load_chunk)
    {
      std::cout << "Failed to load sample" << std::endl;
    }
    
    out_samples[i].chunk = load_chunk;
  }
#endif
}


void
play_nodes(const float ear[3],
           const Node_sample_3d nodes[],
           const size_t number_of_nodes)
{
#ifndef WIN32
  for(size_t i = 0; i < number_of_nodes; ++i)
  { 
    if(Mix_PlayChannel(get_free_channel(), nodes[i].chunk_to_play, 0)==-1)
    {
      printf("Mix_PlayChannel: %s\n",Mix_GetError());
    }
  }
#endif
}


void
play_nodes(const Node_sample_2d nodes[],
          const size_t number_of_nodes)
{
#ifndef WIN32
  for(size_t i = 0; i < number_of_nodes; ++i)
  { 
    if(Mix_PlayChannel(get_free_channel(), nodes[i].chunk_to_play, 0)==-1)
    {
      printf("Mix_PlayChannel: %s\n",Mix_GetError());
    }
  }
#endif
}


} // ns