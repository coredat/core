#include <core/resources/shader.hpp>
#include <common/error_strings.hpp>
#include <data/global_data/resource_data.hpp>
#include <data/global_data/shader_data.hpp>
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
  
    auto data = Resource_data::get_resources();
    assert(data);
  
    util::generic_id return_id = util::generic_id_invalid();

    // Check if shader already exists.
    {
      Resource_data::data_lock(data->shader_data);
      
      util::generic_id search_id = util::generic_id_invalid();
      Resource_data::shader_data_search_property_name(data->shader_data, shader_name, &search_id);
      
      Resource_data::data_unlock(data->shader_data);
    
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
      Resource_data::data_lock(data->shader_data);
      
      const util::generic_id id = Resource_data::shader_data_push_back(data->shader_data);
      Resource_data::shader_data_set_property_name(data->shader_data, id, shader_name);
      Resource_data::shader_data_set_property_shader(data->shader_data, id, shader);
      
      return_id = id;
      
      Resource_data::data_unlock(data->shader_data);
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
  auto shader_data = Resource_data::get_resources()->shader_data;
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