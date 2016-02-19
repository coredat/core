#ifndef AUDIO_INCLUDED_0FE36BE2_D7D3_4683_8473_DFF362E0990C
#define AUDIO_INCLUDED_0FE36BE2_D7D3_4683_8473_DFF362E0990C


#include <stddef.h>
#include <SDL_mixer.h>


namespace Audio {


struct Node_sample_3d
{
  Mix_Chunk   *chunk_to_play;
  float       position[3];
};


struct Node_sample_2d
{
  Mix_Chunk   *chunk_to_play;
  float       volume;
};


struct Node_song
{
  Mix_Chunk   *chunk_to_play;
  float       volume;
};



void initialize();
void de_initialize();

void play_nodes(const float ear[3], const Node_sample_3d[], size_t number_of_nodes);
void play_nodes(const Node_sample_2d[], size_t number_of_nodes);
void play_nodes(const Node_song[], size_t number_of_nodes);



inline void
play()
{
  static Mix_Chunk *sample;
  sample = Mix_LoadWAV("/Users/PhilCK/Developer/wired/assets/audio/temp_shot.wav");
  
  if(!sample)
  {
      printf("Mix_LoadWAV: %s\n", Mix_GetError());
      // handle error
  }

  // play sample on first free unreserved channel
  // play it exactly once through
  // Mix_Chunk *sample; //previously loaded
  if(Mix_PlayChannel(-1, sample, 0)==-1)
  {
    printf("Mix_PlayChannel: %s\n",Mix_GetError());
  }
}


} // ns


#endif // inc guard