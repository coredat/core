#include "audio.hpp"


namespace
{

}


namespace Audio {


void
initialize()
{
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
  
  Mix_AllocateChannels(256);
}


void
de_initialize()
{
  while(Mix_Init(0))
  {
    Mix_Quit();
  }
}


void
play_nodes(const float ear[3], const Node_sample_3d[], size_t number_of_nodes)
{
}


void
play_nodes(const Node_sample_2d[], size_t number_of_nodes)
{
}


void
play_nodes(const Node_song[], size_t number_of_nodes)
{
}



} // ns