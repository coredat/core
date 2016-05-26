#include <data/global_data/shader_data.hpp>
#include <graphics_api/utils/shader_utils.hpp>
#include <common/error_strings.hpp>
#include <utilities/logging.hpp>
#include <utilities/string_helpers.hpp>
#include <assert.h>


namespace
{
  constexpr uint32_t max_shader_name = 256;
}


namespace Resource_data {


void
lock(Shader_data *data)
{
}


void
unlock(Shader_data *data)
{
}


void
shader_data_init(Shader_data *data)
{
  assert(data);

  lock(data);

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
  
  unlock(data);
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
  
  lock(data);

  Ogl::Shader new_shader;
  Ogl::shader_create(&new_shader, vs, gs, ps);
  
  const uint32_t index = data->size;
  const util::generic_id id = data->size + 1;
  
  ++(data->size);
  
  if(Ogl::shader_is_valid(new_shader))
  {
    const std::string filename = util::get_filename_from_path(name);
  
    data->shader_ids[index] = id;
    strlcpy(&data->shader_name[index * max_shader_name], filename.c_str(), sizeof(char) * max_shader_name);
    data->shader[index] = new_shader;
  }
  else
  {
    assert(false);
    LOG_ERROR(Error_string::resource_not_found());
  }
  
  unlock(data);

  return id;
}


const char*
shader_data_get_name(Shader_data *data,
                     const util::generic_id id)
{
  assert(data);
  assert(id < data->size);

  const char *name = nullptr;

  lock(data);
  
  name = &data->shader_name[id * max_shader_name];
  
  unlock(data);
  
  return name;
}


} // ns