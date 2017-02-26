#include <core/resources/shader.hpp>
#include <common/error_strings.hpp>
#include <common/fixed_string_search.hpp>
#include <data/context_data.hpp>
#include <data/context/shader_data.hpp>
#include <utilities/string.hpp>
#include <graphics_api/utils/shader_utils.hpp>
#include <graphics_api/ogl/ogl_shader.hpp>
#include <assert.h>


namespace Core {


struct Shader::Impl
{
  uint32_t id = 0;
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
  inline uint32_t
  push_new_shader(const char *shader_name,
                  const char *vs_code,
                  const char *gs_code,
                  const char *ps_code)
  {
    assert(vs_code && gs_code && shader_name);
  
    auto data = Data::get_context_data();
    assert(data);
    
    auto shd_data = data->shader_data;
    assert(shd_data);
  
    uint32_t return_id = 0;

    // Check if shader already exists.
    {
      Data::data_lock(shd_data);
      
      size_t search_index(0);
      
      if(Common::fixed_string_search(shader_name,
                                     Data::shader_get_name_data(shd_data),
                                     Data::shader_get_name_stride(),
                                     Data::shader_get_size(shd_data),
                                     &search_index))
      {
        return_id = shd_data->keys[search_index];
      }
      else
      {
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
          
          const uint32_t id = Data::shader_push(data->shader_data);
          Data::shader_set_name(data->shader_data, id, shader_name, strlen(shader_name));
          Data::shader_set_shader(data->shader_data, id, &shader);
          
          Data::data_unlock(data->shader_data);
          
          return_id = id;
        }

      }
      
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
    if(!lib::file::exists(filename))
    {
      LOG_ERROR(Error_string::file_not_found());
      return;
    }
  
    const std::string name = lib::string::get_dir_from_filepath(filename);
    
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
  auto shader_data = Data::get_context_data()->shader_data;
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


Shader&
Shader::operator=(const Shader &other)
{
  this->m_impl->id = other.m_impl->id;
  
  return *this;
}


bool
Shader::is_valid() const
{
  return m_impl->id != 0;
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
