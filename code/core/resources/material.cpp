#include <core/resources/material.hpp>
#include <core/resources/shader.hpp>
#include <core/resources/texture.hpp>
#include <common/error_strings.hpp>
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


Material::Material(const uint32_t id)
: m_impl(new Impl{id})
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
  assert(m_impl);

  if(!shader.is_valid())
  {
    assert(false);
    LOG_ERROR(Error_string::resource_is_invalid());
    return;
  }

  if(exists())
  {
    auto resource = Resource_data::get_resources();
    assert(resource);
  
    auto data = resource->material_data;
    assert(data);
    
    Resource_data::data_lock(data);
    
    Resource_data::Material_detail material;
    Resource_data::material_data_get_property_material(data, m_impl->material_id, &material);
    
    material.shader_id = shader.get_id();
    
    Resource_data::material_data_set_property_material(data, m_impl->material_id, material);
    
    Resource_data::data_unlock(data);
  }
  else
  {
    assert(false);
    LOG_ERROR(Error_string::resource_not_found());
    return;
  }
}


void
Material::set_map_01(const Texture &texture)
{
  assert(m_impl);
  
  if(!texture.exists())
  {
    assert(false);
    LOG_ERROR(Error_string::resource_is_invalid());
    return;
  }

  if(exists())
  {
    auto resource = Resource_data::get_resources();
    assert(resource);
  
    auto data = resource->material_data;
    assert(data);
    
    Resource_data::data_lock(data);
    
    Resource_data::Material_detail material;
    Resource_data::material_data_get_property_material(data, m_impl->material_id, &material);
    
    material.texture_map_01_id = texture.get_id();
    
    Resource_data::material_data_set_property_material(data, m_impl->material_id, material);
    
    Resource_data::data_unlock(data);
  }
  else
  {
    assert(false);
    LOG_ERROR(Error_string::resource_not_found());
    return;
  }
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


bool
Material::exists() const
{
  assert(m_impl);
  
  return m_impl->material_id > 0;
}


Material::operator bool() const
{
  return exists();
}


uint32_t
Material::get_id() const
{
   assert(m_impl);
  
   return m_impl->material_id;
}


} // ns