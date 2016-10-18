#include <core/resources/material.hpp>
#include <core/resources/shader.hpp>
#include <core/resources/texture.hpp>
#include <core/color/color.hpp>
#include <core/color/color_utils.hpp>
#include <systems/renderer_material/material.hpp>
#include <common/error_strings.hpp>
#include <data/context_data.hpp>
#include <data/context/texture_data.hpp>
#include <data/context/shader_data.hpp>
#include <data/context/material_data.hpp>
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

    auto resources = Data::get_context_data();
    assert(resources);
    
    auto data = resources->material_data;
    assert(data);
    
    Data::data_lock(data);
    
    const util::generic_id id = Data::material_push(data);
    Data::material_set_name(data, id, name, strlen(name));
    
    // Set instance id in the hash key
    {
      ::Material_renderer::Material_id priority_key;
      Data::material_get_material_hash(data, id, &priority_key);
      
      priority_key.material_instance = data->size;
      Data::material_set_material_hash(data, id, &priority_key);
    }
    
    m_impl->material_id = id;
    
    Data::data_unlock(data);
    
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
    auto resource = Data::get_context_data();
    assert(resource);
  
    auto mat_data = resource->material_data;
    auto shd_data = resource->shader_data;
    assert(mat_data);
    assert(shd_data);
    
    // Get shader data and the uniforms
    Ogl::Shader_uniforms uniforms;
    Ogl::Shader shd;
    {
      Data::data_lock(shd_data);
      
      Data::shader_get_shader(shd_data, shader.get_id(), &shd);
      
      Ogl::shader_uniforms_retrive(&uniforms, &shd);
      
      Data::data_unlock(shd_data);
    }
    
    // Setup Material
    {
      Data::data_lock(mat_data);
      
      // Create and add the shader part of the material
      {
        ::Material_renderer::Material material;
        Data::material_get_material(mat_data, m_impl->material_id, &material);
        ::Material_renderer::create_material(&material, &shd);
        Data::material_set_material(mat_data, m_impl->material_id, &material);
      }
      
      // Update the hash key
      {
        ::Material_renderer::Material_id material_hash;
        Data::material_get_material_hash(mat_data, m_impl->material_id, &material_hash);
        material_hash.shader_id = shader.get_id();
        Data::material_set_material_hash(mat_data, m_impl->material_id, &material_hash);
      }
      
      Data::data_unlock(mat_data);
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
    auto resource = Data::get_context_data();
    assert(resource);
  
    auto mat_data = resource->material_data;
    assert(mat_data);
    
    auto tex_data = resource->texture_data;
    assert(tex_data);
    
    // Get texture
    Ogl::Texture out_texture;
    {
      Data::data_lock(tex_data);
      
      const bool found_texture = Data::texture_get_texture(tex_data, texture.get_id(), &out_texture);
      assert(found_texture);
      
      Data::data_unlock(tex_data);
    }
    
    // Update material
    {
      Data::data_lock(mat_data);
      
      ::Material_renderer::Material out_material;
      Data::material_get_material(mat_data, m_impl->material_id, &out_material);
      
      out_material.map_01 = out_texture;
      out_material.map_01_id = texture.get_id();
      Data::material_set_material(mat_data, m_impl->material_id, &out_material);

      Data::data_unlock(mat_data);
    }
    
    // Update the hash key
    {
      ::Material_renderer::Material_id material_hash;
      Data::material_get_material_hash(mat_data, m_impl->material_id, &material_hash);
      material_hash.texture_map_01_id = texture.get_id();
      Data::material_set_material_hash(mat_data, m_impl->material_id, &material_hash);
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

  auto resource = Data::get_context_data();
  assert(resource);

  auto mat_data = resource->material_data;
  assert(mat_data);

  auto tex_data = resource->texture_data;
  assert(tex_data);
  
  Data::data_lock(mat_data);
  Data::data_lock(tex_data);
  
  ::Material_renderer::Material mat;
  Data::material_get_material(mat_data, m_impl->material_id, &mat);
  
  const util::generic_id map_id = mat.map_01_id;
  
  Data::data_unlock(mat_data);
  Data::data_unlock(tex_data);
  
  return Core::Texture(map_id);
}


void
Material::set_color(const Color color)
{
  assert(m_impl);

  if(exists())
  {
    auto resource = Data::get_context_data();
    assert(resource);
  
    auto mat_data = resource->material_data;
    assert(mat_data);
    
    // Update material
    {
      Data::data_lock(mat_data);
      
      ::Material_renderer::Material out_material;
      Data::material_get_material(mat_data, m_impl->material_id, &out_material);
      
      Core::Color_utils::to_float_array(color, out_material.color_data);
      Data::material_set_material(mat_data, m_impl->material_id, &out_material);

      Data::data_unlock(mat_data);
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
  
  auto data = Data::get_context_data()->material_data;
  assert(data);

  const char *name = "";
  
  Data::material_get_name(data, m_impl->material_id, &name);
  
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