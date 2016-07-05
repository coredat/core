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
  
  static Texture_data texture;
  texture_data_init(&texture, 256);
  data->texture_data = &texture;
  
  static Mesh_data model;
  mesh_data_init(&model, 256);
  data->mesh_data = &model;
  
  static Shader_data shaders;
  shader_data_init(&shaders, 128);
  data->shader_data = &shaders;
  
  static Material_data materials;
  material_data_init(&materials, 256);
  data->material_data = &materials;
  
  static Post_process_data post_data;
  post_process_data_init(&post_data, 32);
  data->post_data = &post_data;
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