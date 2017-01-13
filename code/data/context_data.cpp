#include <data/context_data.hpp>
#include <data/context/font_data.hpp>
#include <data/context/font_glyph_data.hpp>
#include <data/context/material_data.hpp>
#include <data/context/mesh_data.hpp>
#include <data/context/post_process_data.hpp>
#include <data/context/shader_data.hpp>
#include <data/context/text_mesh_data.hpp>
#include <data/context/texture_data.hpp>
#include <op/op.hpp>
#include <utilities/logging.hpp>


namespace {

std::shared_ptr<Data::Context> context_data;

} // anon ns


namespace Data {


Context::Context(const Context_data_setup &setup)
{
  LOG_TODO_ONCE("Remove these statics");

  static Data::Texture_data texture;
  Data::texture_create(&texture, 256);
  this->texture_data = &texture;
  
  static Data::Mesh_data model;
  Data::mesh_create(&model, 256);
  this->mesh_data = &model;
  
  static Data::Shader_data shaders;
  Data::shader_create(&shaders, 128);
  this->shader_data = &shaders;
  
  static Data::Material_data materials;
  Data::material_create(&materials, 256);
  this->material_data = &materials;
  
  static Data::Post_process_data post_data;
  Data::post_process_create(&post_data, 32);
  this->post_process_data = &post_data;
  
  static Data::Text_mesh_data text_mesh_data;
  Data::text_mesh_create(&text_mesh_data, 1024);
  this->text_mesh_data = &text_mesh_data;
  
  static Data::Font_data font_data;
  Data::font_create(&font_data, 32);
  this->font_data = &font_data;
  
  static Data::Font_glyph_data glyphs;
  Data::font_glyph_create(&glyphs, 2048);
  this->font_glyph_data = &glyphs;
  
   // -- Setup Buffer -- //
  static uint32_t alloc_count = 0;

  opCallbackAlloc([](size_t requested_size, uintptr_t user_data)
  {
    *reinterpret_cast<uint32_t*>(user_data) += 1;
    return malloc(requested_size);
  });

  opCallbackResize([](size_t requested_size, void *old_data, uintptr_t user_data)
  {
    *reinterpret_cast<uint32_t*>(user_data) += 1;
    return realloc(old_data, requested_size);
  });

  opCallbackDestroy([](void *data, uintptr_t user_data)
  {
    *reinterpret_cast<uint32_t*>(user_data) += 1;
    free(data);
  });

  opCallbackUserData((uintptr_t)&alloc_count); 
  
  op_context = opContextCreate();
  op_buffer  = opBufferCreate();
}


Context::~Context()
{
  Data::font_destroy(font_data);
  Data::font_glyph_destroy(font_glyph_data);
  Data::material_destroy(material_data);
  Data::mesh_destroy(mesh_data);
  Data::post_process_destroy(post_process_data);
  Data::shader_destroy(shader_data);
  Data::text_mesh_destroy(text_mesh_data);
  Data::texture_destroy(texture_data);
}



void
init_context_data(const Context_data_setup &setup)
{
  context_data.reset(new Data::Context(setup));
}


std::shared_ptr<Context>
get_context_data()
{
  #ifndef NDEBUG
  if(!context_data)
  {
    LOG_ERROR("Context data needs to be explicitly setup. It will fail in release mode.");
    init_context_data(Context_data_setup{});
  }
  #endif
  
  return context_data;
}


} // ns