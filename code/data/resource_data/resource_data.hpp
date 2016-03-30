#ifndef RESOURCE_DATA_INCLUDED_
#define RESOURCE_DATA_INCLUDED_


#include "audio_pool.hpp"
#include "audio_raw_pool.hpp"
#include "model_pool.hpp"
#include "texture_pool.hpp"


namespace Resource_data {


struct Resources
{
  Texture_pool  *texture_pool = nullptr;
  Audio_pool    *audio_pool   = nullptr;
  Model_pool    *model_pool   = nullptr;
};


} // ns


#endif // inc guard