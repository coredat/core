#include <core/resources/texture.hpp>
#include <data/global_data/resource_data.hpp>
#include <common/error_strings.hpp>
#include <utilities/logging.hpp>
#include <utilities/string_helpers.hpp>
#include <transformations/texture/create_texture.hpp>


namespace Core {


struct Texture::Impl
{
  util::generic_id texture_id;
};


Texture::Texture()
: Texture(util::generic_id_invalid())
{
}


Texture::Texture(const util::generic_id id)
: m_impl(new Impl{id})
{
  if(!id)
  {
    return;
  }

  // Check the id is valid, null it if it isn't
  {
    Resource_data::Resources *resources = Resource_data::get_resources();
    assert(resources);
    
    Resource_data::Texture_data *texture_data = resources->texture_data;
    assert(texture_data);

    Resource_data::data_lock(texture_data);
    
    if(!Resource_data::texture_data_exists(texture_data, id))
    {
      LOG_WARNING(Error_string::resource_is_invalid());
      m_impl->texture_id = util::generic_id_invalid();
    }

    Resource_data::data_unlock(texture_data);
  }
}


Texture::Texture(const char *filepath)
: m_impl(new Impl)
{
  const std::string file(filepath);
  const std::string name(util::get_filename_from_path(filepath));

  if(!util::file::exists(filepath))
  {
    LOG_ERROR(Error_string::file_not_found());
    return;
  }

  Resource_data::Resources *resources = Resource_data::get_resources();
  assert(resources);

  auto tex_data = Resource_data::get_resources()->texture_data;
  assert(tex_data);
  
  // Search to see if we have already loaded the texture.
  {
    Resource_data::data_lock(tex_data);
    
    util::generic_id search_id = util::generic_id_invalid();
    
    if(Resource_data::texture_data_search_property_name(tex_data, name.c_str(), &search_id))
    {
      #ifdef LOG_DOUBLE_RESOURCE_INITIALIZE
      LOG_WARNING(Error_string::resource_already_exists());
      #endif
      
      m_impl->texture_id = search_id;
    }
    
    Resource_data::data_unlock(tex_data);
    
    if(search_id)
    {
      return;
    }
  }
  
  // Load up a new texture
  {
    Ogl::Texture new_texture;
    Texture_utils::create_texture_from_file(filepath, &new_texture);
   
    // Add to pool
    if(Ogl::texture_is_valid(&new_texture))
    {
      Resource_data::data_lock(tex_data);
      
      const util::generic_id id = Resource_data::texture_data_push_back(tex_data);
      Resource_data::texture_data_set_property_name(tex_data, id, name.c_str());
      Resource_data::texture_data_set_property_texture(tex_data, id, new_texture);
      
      m_impl->texture_id = id;
      
      Resource_data::data_unlock(tex_data);
    }
    else
    {
      LOG_ERROR(Error_string::failed_to_create_resource());
    }
  }
}


Texture::Texture(const uint32_t width,
                 const uint32_t height,
                 const void *data)
: m_impl(new Impl)
{
  // Load up a new texture
  {
    Ogl::Texture new_texture;
    Ogl::texture_create_2d(&new_texture, width, height, GL_RGBA, false, data);
   
    auto tex_data = Resource_data::get_resources()->texture_data;
    assert(tex_data);
   
    // Add to pool
    if(Ogl::texture_is_valid(&new_texture))
    {
      Resource_data::data_lock(tex_data);
      
      const util::generic_id id = Resource_data::texture_data_push_back(tex_data);
      Resource_data::texture_data_set_property_name(tex_data, id, "user_defined");
      Resource_data::texture_data_set_property_texture(tex_data, id, new_texture);
      
      m_impl->texture_id = id;
      
      Resource_data::data_unlock(tex_data);
    }
    else
    {
      LOG_ERROR(Error_string::failed_to_create_resource());
    }
  }
}


Texture::~Texture()
{
}


Texture::Texture(const Texture &other)
: m_impl(new Impl{other.m_impl->texture_id})
{
}


Texture::Texture(Texture &&other)
: m_impl(new Impl{other.m_impl->texture_id})
{
}


Texture&
Texture::operator=(const Texture &other)
{
  m_impl.reset(new Impl);
  m_impl->texture_id = other.m_impl->texture_id;
  return *this;
}


Texture&
Texture::operator=(Texture &&other)
{
  m_impl.reset(new Impl);
  m_impl->texture_id = other.m_impl->texture_id;
  return *this;
}


namespace
{
  Ogl::Texture
  get_texture_data(const util::generic_id id)
  {
    Resource_data::Resources *resources = Resource_data::get_resources();
    assert(resources);

    auto tex_data = Resource_data::get_resources()->texture_data;
    assert(tex_data);

    Ogl::Texture tex;
    
    Resource_data::data_lock(tex_data);
    Resource_data::texture_data_get_property_texture(tex_data, id, &tex);
    Resource_data::data_unlock(tex_data);
    
    return tex;
  }
}


void
Texture::update_sub_texture(const uint32_t x_offset,
                            const uint32_t y_offset,
                            const uint32_t width,
                            const uint32_t height,
                            const void *data)
{
  Ogl::Texture get_texture = get_texture_data(m_impl->texture_id);
  Ogl::texture_update_texture_2d(&get_texture, x_offset, y_offset, width, height, data);
  
  auto err = glGetError();
  if(err)
  {
    LOG_ERROR("Failed update");
  }
}


uint32_t
Texture::get_width() const
{
  assert(m_impl);
  
  return get_texture_data(m_impl->texture_id).width;
}


uint32_t
Texture::get_height() const
{
  assert(m_impl);
  
  return get_texture_data(m_impl->texture_id).height;
}


bool
Texture::exists() const
{
  return !!m_impl->texture_id;
}


Texture::operator bool() const
{
  return exists();
}


util::generic_id
Texture::get_id() const
{
  if(m_impl)
  {
    return m_impl->texture_id;
  }

  return 0;
}


  } // ns