#include <core/resources/material.hpp>
#include <data/global_data/resource_data.hpp>
#include <assert.h>


namespace Core {


struct Material::Impl
{
  util::generic_id material_id = util::generic_id_invalid();
};


Material::Material()
: Material("")
{
}


Material::Material(const char *name)
: m_impl(new Impl())
{
  // If we have a name, then add the material.
  if(strcmp(name, ""))
  {
    assert(m_impl && name);

    auto resources = Resource_data::get_resources();
    assert(resources);
    
    auto data = resources->material_data;
    assert(data);
    
    Resource_data::data_lock(data);
    
    Resource_data::material_data_push_back(data, data->size + 1);
    Resource_data::material_data_set_property_name(data, data->size, name);
    
    m_impl->material_id = data->size;
    
    Resource_data::data_unlock(data);
    
    assert(m_impl->material_id);
  }
}


Material::~Material()
{
}


Material::Material(const Material &mat)
{
}


Material::Material(Material &&mat)
{
}

Material&
Material::operator=(const Material &mat)
{
  return *this;
}


Material&
Material::operator=(Material &&mat)
{
  return *this;
}


void
Material::set_shader(const Shader &shader)
{
}


void
Material::set_map_01(const Texture &texture)
{
}


void
Material::set_map_02(const Texture &texture)
{
}


void
Material::set_map_03(const Texture &texture)
{
}


const char *
Material::get_name() const
{
  assert(m_impl);
  
  auto data = Resource_data::get_resources()->material_data;
  assert(data);

  const char *name = "";
  
  Resource_data::material_data_get_property_name(data, m_impl->material_id, &name);
  
  return name;
}



} // ns