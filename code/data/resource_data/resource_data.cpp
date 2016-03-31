#include <data/resource_data/resource_data.hpp>
#include <assert.h>


namespace
{
  Resource_data::Resources *data = nullptr;
}


namespace Resource_data {


void
resources_init(Resources *resources)
{
  assert(resources);
  
  static Audio_pool audio;
  audio_pool_init(&audio);
  
  static Texture_pool texture;
  texture_pool_init(&texture);
  
  static Mesh_pool model;
  mesh_pool_init(&model);
  
  resources->audio_pool   = &audio;
  resources->texture_pool = &texture;
  resources->mesh_pool    = &model;
  
  data = resources;
}


Resources*
get_horrible_hack_resouces()
{
  return data;
}


} // ns