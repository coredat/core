#include <data/global_data/resource_data.hpp>
#include <graphics_api/initialize.hpp>
#include <assert.h>


namespace
{
  Resource_data::Resources *data = nullptr;
}


namespace Resource_data {


void
resources_init()
{
  static Resources resource_data;
  data = &resource_data;
  
  static Audio_pool audio;
  audio_pool_init(&audio);
  
  static Texture_pool texture;
  texture_pool_init(&texture);
  
  static Mesh_pool model;
  mesh_pool_init(&model);
  
  data->audio_pool   = &audio;
  data->texture_pool = &texture;
  data->mesh_pool    = &model;
}


Resources*
get_resources()
{
  if(!data)
  {
    resources_init();
  }

  return data;
}


} // ns