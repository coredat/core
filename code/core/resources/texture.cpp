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
: Texture((uint32_t) 0)
{
}


Texture::Texture(const uint32_t id)
: m_impl(new Impl{id})
{
  LOG_TODO("Check this id is valid");
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
      
      assert(Resource_data::texture_data_push_back(tex_data, tex_data->size + 1));
      Resource_data::texture_data_set_property_name(tex_data, tex_data->size, name.c_str());
      Resource_data::texture_data_set_property_texture(tex_data, tex_data->size, new_texture);
      
      m_impl->texture_id = tex_data->size;
      
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


bool
Texture::exists() const
{
  return !!m_impl->texture_id;
}


Texture::operator bool() const
{
  return exists();
}


uint32_t
Texture::get_id() const
{
  if(m_impl)
  {
    return m_impl->texture_id;
  }

  return 0;
}


  } // ns