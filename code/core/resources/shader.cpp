#include <core/resources/shader.hpp>
#include <data/global_data/resource_data.hpp>
#include <utilities/generic_id.hpp>
#include <assert.h>


namespace Core {


struct Shader::Impl
{
  util::generic_id id = 0;
};


Shader::Shader(const char *filename)
: m_impl(new Impl)
{
  auto shader_data = Resource_data::get_resources()->shader_data;
  assert(shader_data);
  
  m_impl->id = Resource_data::shader_data_add_from_file(shader_data, filename);
}


Shader::~Shader()
{
}


bool
Shader::is_valid() const
{
  return m_impl->id > 0;
}


} // ns