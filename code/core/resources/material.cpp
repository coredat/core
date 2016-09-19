#include <core/resources/material.hpp>
#include <core/resources/shader.hpp>
#include <core/resources/texture.hpp>
#include <core/color/color.hpp>
#include <core/color/color_utils.hpp>
#include <systems/renderer_material/material.hpp>
#include <common/error_strings.hpp>
#include <data/global_data/resource_data.hpp>
#include <graphics_api/ogl/ogl_shader_uniform.hpp>
#include <assert.h>


namespace Core {


struct Material::Impl
{
  util::generic_id material_id;
};


Material::Material()
: Material("")
{
}


Material::Material(const util::generic_id id)
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
    
    const util::generic_id id = Resource_data::material_data_push_back(data);
    Resource_data::material_data_set_property_name(data, id, name);
    
    // Set instance id in the hash key
    {
      ::Material_renderer::Material_id priority_key;
      Resource_data::material_data_get_property_material_hash_id(data, id, &priority_key);
      
      priority_key.material_instance = data->size;
      Resource_data::material_data_set_property_material_hash_id(data, id, priority_key);
    }
    
    m_impl->material_id = id;
    
    Resource_data::data_unlock(data);
    
    assert(m_impl->material_id);
  }
}


Material::~Material()
{
}


Material::Material(const Material &mat)
: m_impl(new Impl{mat.m_impl->material_id})
{
}


Material::Material(Material &&mat)
: m_impl(new Impl{mat.m_impl->material_id})
{
}


Material&
Material::operator=(const Material &mat)
{
  m_impl->material_id = mat.m_impl->material_id;
  return *this;
}


Material&
Material::operator=(Material &&mat)
{
  m_impl->material_id = mat.m_impl->material_id;
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
  
    auto mat_data = resource->material_data;
    auto shd_data = resource->shader_data;
    assert(mat_data);
    assert(shd_data);
    
    // Get shader data and the uniforms
    Ogl::Shader_uniforms uniforms;
    Ogl::Shader shd;
    {
      Resource_data::data_lock(shd_data);
      
      Resource_data::shader_data_get_property_shader(shd_data, shader.get_id(), &shd);
      
      Ogl::shader_uniforms_retrive(&uniforms, &shd);
      
      Resource_data::data_unlock(shd_data);
    }
    
    // Setup Material
    {
      Resource_data::data_lock(mat_data);
      
      // Create and add the shader part of the material
      {
        ::Material_renderer::Material *material;
        Resource_data::material_data_get_property_material(mat_data, m_impl->material_id, &material);
        ::Material_renderer::create_material(material, &shd);
        Resource_data::material_data_set_property_material(mat_data, m_impl->material_id, material);
      }
      
      // Update the hash key
      {
        ::Material_renderer::Material_id material_hash;
        Resource_data::material_data_get_property_material_hash_id(mat_data, m_impl->material_id, &material_hash);
        material_hash.shader_id = shader.get_id();
        Resource_data::material_data_set_property_material_hash_id(mat_data, m_impl->material_id, material_hash);
      }
      
      Resource_data::data_unlock(mat_data);
    }

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
  
    auto mat_data = resource->material_data;
    assert(mat_data);
    
    auto tex_data = resource->texture_data;
    assert(tex_data);
    
    // Get texture
    Ogl::Texture out_texture;
    {
      Resource_data::data_lock(tex_data);
      
      const bool found_texture = Resource_data::texture_data_get_property_texture(tex_data, texture.get_id(), &out_texture);
      assert(found_texture);
      
      Resource_data::data_unlock(tex_data);
    }
    
    // Update material
    {
      Resource_data::data_lock(mat_data);
      
      ::Material_renderer::Material *out_material;
      Resource_data::material_data_get_property_material(mat_data, m_impl->material_id, &out_material);
      
      out_material->map_01 = out_texture;
      out_material->map_01_id = texture.get_id();
      Resource_data::material_data_set_property_material(mat_data, m_impl->material_id, out_material);

      Resource_data::data_unlock(mat_data);
    }
    
    // Update the hash key
    {
      ::Material_renderer::Material_id material_hash;
      Resource_data::material_data_get_property_material_hash_id(mat_data, m_impl->material_id, &material_hash);
      material_hash.texture_map_01_id = texture.get_id();
      Resource_data::material_data_set_property_material_hash_id(mat_data, m_impl->material_id, material_hash);
    }
  }
  else
  {
    assert(false);
    LOG_ERROR(Error_string::resource_not_found());
    return;
  }
}


Texture
Material::get_map_01() const
{
  assert(m_impl);
  
  if(!m_impl->material_id)
  {
    LOG_WARNING("No texture on this material.");
    return Core::Texture();
  }

  auto resource = Resource_data::get_resources();
  assert(resource);

  auto mat_data = resource->material_data;
  assert(mat_data);

  auto tex_data = resource->texture_data;
  assert(tex_data);
  
  Resource_data::data_lock(mat_data);
  Resource_data::data_lock(tex_data);
  
  ::Material_renderer::Material *mat = nullptr;
  Resource_data::material_data_get_property_material(mat_data, m_impl->material_id, &mat);
  
  const util::generic_id map_id = mat->map_01_id;
  
  Resource_data::data_unlock(mat_data);
  Resource_data::data_unlock(tex_data);
  
  return Core::Texture(map_id);
}


void
Material::set_color(const Color color)
{
  assert(m_impl);

  if(exists())
  {
    auto resource = Resource_data::get_resources();
    assert(resource);
  
    auto mat_data = resource->material_data;
    assert(mat_data);
    
    // Update material
    {
      Resource_data::data_lock(mat_data);
      
      ::Material_renderer::Material *out_material;
      Resource_data::material_data_get_property_material(mat_data, m_impl->material_id, &out_material);
      
      Core::Color_utils::to_float_array(color, out_material->color_data);
      Resource_data::material_data_set_property_material(mat_data, m_impl->material_id, out_material);

      Resource_data::data_unlock(mat_data);
    }
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