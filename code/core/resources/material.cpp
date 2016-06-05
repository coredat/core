#include <core/resources/material.hpp>
#include <data/global_data/resource_data.hpp>


namespace Core {


struct Material::Impl
{
  util::generic_id material_id = 0;
};


Material::Material(const char *name)
: m_impl(new Impl())
{
  assert(m_impl);

  auto resources = Resource_data::get_resources();
  assert(resources);
  
  auto data = resources->material_data;
  
  m_impl->material_id = Resource_data::material_data_add(data, name, 0, 0);
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

  return Resource_data::material_data_get_name(data, m_impl->material_id);
}



} // ns