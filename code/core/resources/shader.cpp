#include <core/resources/shader.hpp>
#include <common/error_strings.hpp>
#include <data/global_data/resource_data.hpp>
#include <data/context/shader_data.hpp>
#include <utilities/generic_id.hpp>
#include <utilities/string_helpers.hpp>
#include <graphics_api/utils/shader_utils.hpp>
#include <graphics_api/ogl/ogl_shader.hpp>
#include <assert.h>


namespace Core {


struct Shader::Impl
{
  util::generic_id id = util::generic_id_invalid();
};


Shader::Shader()
: Shader("")
{
}


namespace
{

  namespace Gfx_util = Graphics_api::Util;

  /*
    This inserts the data into the shader_data resource.
  */
  inline util::generic_id
  push_new_shader(const char *shader_name,
                  const char *vs_code,
                  const char *gs_code,
                  const char *ps_code)
  {
    assert(vs_code && gs_code && shader_name);
  
    auto data = Resource_data::get_resource_data();
    assert(data);
  
    util::generic_id return_id = util::generic_id_invalid();

    // Check if shader already exists.
    {
      Data::data_lock(data->shader_data);
      
      auto
      search_name = [](const auto *data, const char *value, util::generic_id *out_key) -> bool
      {
        LOG_TODO_ONCE("This is a hack solve it.");
        bool found = false;

        for(size_t i = 0; i < data->size; ++i)
        {
          if(!strcmp(value, &data->field_name[i * 32]))
          {
            found = true;

            if(out_key)
            {
              *out_key = data->keys[i];
            }

            break;
          }
        }

        return found;
      };
      
      util::generic_id search_id = util::generic_id_invalid();
      search_name(data->shader_data, shader_name, &search_id);
      
      Data::data_unlock(data->shader_data);
    
      if(search_id)
      {
        return_id = search_id;
        return return_id;
      }
    }
    
  const auto errd = glGetError();
  
  if(errd)
  {
    int i = 0;
  }
    
    
    Ogl::Shader shader;
    Ogl::shader_create(&shader,
                       vs_code,
                       gs_code,
                       ps_code);
    
    assert(Ogl::shader_is_valid(&shader));
    
    // Add the new shader
    if(Ogl::shader_is_valid(&shader));
    {
      Data::data_lock(data->shader_data);
      
      const util::generic_id id = Data::shader_push(data->shader_data);
      Data::shader_set_name(data->shader_data, id, shader_name, strlen(shader_name));
      Data::shader_set_shader(data->shader_data, id, &shader);
      
      return_id = id;
      
      Data::data_unlock(data->shader_data);
    }
    
    return return_id;
  }

}


Shader::Shader(const char *filename)
: m_impl(new Impl)
{
  assert(filename);
  
  if(filename)
  {
    // Check file exists first
    if(!util::file::exists(filename))
    {
      LOG_ERROR(Error_string::file_not_found());
      return;
    }
  
    const std::string name = util::get_filename_from_path(filename);
    
    Gfx_util::Shader_code code = Gfx_util::shader_code_from_tagged_file(filename);
    
    m_impl->id = push_new_shader(name.c_str(),
                                 code.vs_code.c_str(),
                                 code.gs_code.c_str(),
                                 code.ps_code.c_str());
  }
}


Shader::Shader(const char *name, const char *vs, const char *gs, const char *ps)
: m_impl(new Impl)
{
  auto shader_data = Resource_data::get_resource_data()->shader_data;
  assert(shader_data);
  
  m_impl->id = push_new_shader(name, vs, gs, ps);
}


Shader::Shader(const Shader &other)
: m_impl(new Impl)
{
  m_impl->id = other.m_impl->id;
}


Shader::~Shader()
{
}


bool
Shader::is_valid() const
{
  return m_impl->id != util::generic_id_invalid();
}


Shader::operator bool() const
{
  return is_valid();
}


uint32_t
Shader::get_id() const
{
  return m_impl->id;
}


} // ns