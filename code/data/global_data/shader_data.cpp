#include <data/global_data/shader_data.hpp>
#include <graphics_api/utils/shader_utils.hpp>
#include <utilities/logging.hpp>
#include <common/error_strings.hpp>
#include <assert.h>


namespace
{
  constexpr uint32_t max_shader_name = 256;
}


namespace Resource_data {


void
shader_data_init(Shader_data *data)
{
  assert(data);

  const uint32_t number_of_shaders = 10;

  static util::generic_id shader_ids[number_of_shaders];
  data->shader_ids = shader_ids;
  #ifndef NDEBUG
  memset(data->shader_ids, 0, sizeof(shader_ids));
  #endif
  
  static char name_buffer[number_of_shaders * max_shader_name];
  data->shader_name = name_buffer;
  #ifndef NDEBUG
  memset(data->shader_name, 0, sizeof(name_buffer));
  #endif
  
  static Ogl::Shader shaders[number_of_shaders];
  data->shader = shaders;
  #ifndef NDEBUG
  memset(data->shader, 0, sizeof(shaders));
  #endif
  
  data->size = 0;
  
  uint32_t *cap = const_cast<uint32_t*>(&data->capacity);
  *cap = number_of_shaders;
}


util::generic_id
shader_data_add_from_file(Shader_data *data,
                          const char *filename)
{
  assert(data && filename);
  assert(data->size < data->capacity);

  Graphics_api::Util::Shader_code code;
  code = Graphics_api::Util::shader_code_from_tagged_file(filename);

  return shader_data_add_from_code(data,
                                   filename,
                                   code.vs_code.c_str(),
                                   code.gs_code.c_str(),
                                   code.ps_code.c_str());
}


util::generic_id
shader_data_add_from_code(Shader_data *data,
                          const char *name,
                          const char *vs,
                          const char *gs,
                          const char *ps)
{
  assert(data && vs && ps); // gs can be null.
  assert(data->size < data->capacity);

  Ogl::Shader new_shader;
  Ogl::shader_create(&new_shader, vs, gs, ps);
  
  if(Ogl::shader_is_valid(new_shader))
  {
    data->shader_ids[data->size] = data->size + 1;
    strlcpy(&data->shader_name[data->size * max_shader_name], name, sizeof(char) * max_shader_name);
    data->shader[data->size] = new_shader;
    
    ++(data->size);
  }
  else
  {
    assert(false);
    LOG_ERROR(Error_string::resource_not_found());
  }

  return data->size;
}


} // ns