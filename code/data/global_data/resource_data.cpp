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
  data->audio_pool = &audio;
  
  static Texture_pool texture;
  texture_pool_init(&texture);
  data->texture_pool = &texture;
  
  static Mesh_data model;
  mesh_data_init(&model, 256);
  data->mesh_data = &model;
  
  static Shader_data shaders;
  shader_data_init(&shaders, 16);
  data->shader_data = &shaders;
  
  static Material_data materials;
  material_data_init(&materials, 2048);
  data->material_data = &materials;
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