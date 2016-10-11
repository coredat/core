#include <data/global_data/resource_data.hpp>
#include <graphics_api/initialize.hpp>
#include <assert.h>


namespace
{
  // No real technical reason for this being a shared ptr.
  // its just to keep it uniform with the other data collections.
  std::shared_ptr<Resource_data::Resources> data(nullptr);
}


namespace Resource_data {


void
resources_init()
{
  if(!data)
  {
    data.reset(new Resources);
  }
  
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
  
  static Text_mesh_data text_mesh_data;
  text_mesh_data_init(&text_mesh_data, 1024);
  data->text_mesh_data = &text_mesh_data;
  
  static Font_data font_data;
  font_data_init(&font_data, 32);
  data->font_data = &font_data;
  
  static Rasterized_glyphs_data glyphs;
  rasterized_glyphs_data_init(&glyphs, 2048);
  data->glyphs_data = &glyphs;
}


Resources*
get_resources()
{
  if(!data)
  {
    resources_init();
  }

  return data.get();
}


std::shared_ptr<Resource_data::Resources>
get_resource_data()
{
  return data;
}


} // ns