#include <data/global_data/resource_data.hpp>
#include <data/context/texture_data.hpp>
#include <data/context/mesh_data.hpp>
#include <data/context/shader_data.hpp>
#include <data/context/material_data.hpp>
#include <data/context/post_process_data.hpp>
#include <data/context/text_mesh_data.hpp>
#include <data/context/font_data.cpp>
#include <data/context/font_glyph_data.hpp>
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
  
  static Data::Texture_data texture;
  Data::texture_create(&texture, 256);
  data->texture_data = &texture;
  
  static Data::Mesh_data model;
  Data::mesh_create(&model, 256);
  data->mesh_data = &model;
  
  static Data::Shader_data shaders;
  Data::shader_create(&shaders, 128);
  data->shader_data = &shaders;
  
  static Data::Material_data materials;
  Data::material_create(&materials, 256);
  data->material_data = &materials;
  
  static Data::Post_process_data post_data;
  Data::post_process_create(&post_data, 32);
  data->post_data = &post_data;
  
  static Data::Text_mesh_data text_mesh_data;
  Data::text_mesh_create(&text_mesh_data, 1024);
  data->text_mesh_data = &text_mesh_data;
  
  static Data::Font_data font_data;
  Data::font_create(&font_data, 32);
  data->font_data = &font_data;
  
  static Data::Font_glyph_data glyphs;
  Data::font_glyph_create(&glyphs, 2048);
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
  if(!data)
  {
    resources_init();
  }
  
  return data;
}


} // ns