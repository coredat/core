#ifndef AUDIO_INCLUDED_0FE36BE2_D7D3_4683_8473_DFF362E0990C
#define AUDIO_INCLUDED_0FE36BE2_D7D3_4683_8473_DFF362E0990C


#include <stddef.h>
#include <SDL_mixer.h>


namespace Audio {


struct Sample
{
  Mix_Chunk   *chunk = nullptr;
};


struct Node_sample_3d
{
  Mix_Chunk   *chunk_to_play = nullptr;
  float       position[3] = {0,0,0};
};


struct Node_sample_2d
{
  Mix_Chunk   *chunk_to_play = nullptr;
  float       volume = 0;
};



/*!
  Initialize the audio system.
*/
void
initialize();


/*!
  De-init the audio system.
*/
void
de_initialize();


/*!
  Loads a collection of samples into a given buffer.
  \param files_to_load An array of filenames to load.
  \param number_of_files How large the array is.
  \param out_samples[] Where to put the loaded files.
  \param number_of_out_samples How big the output buffer is.
*/
void
load_samples(const char* files_to_load[],
             const size_t number_of_files,
             Sample out_samples[],
             const size_t number_of_out_samples);


/*!
  Play 3d sounds
  \param ear[3] The position of the virtual ear.
  \param nodes[] The chunks to play and where.
  \param number_of_nodes How many nodes to play.
*/
void
play_nodes(const float ear[3],
           const Node_sample_3d nodes[],
           const size_t number_of_nodes);


/*!
  Play 2d sounds.
  \param nodes[] Array of 2d sound nodes.
  \param number_of_nodes How many nodes to play.
*/
void
play_nodes(const Node_sample_2d nodes[],
           const size_t number_of_nodes);


} // ns


#endif // inc guard