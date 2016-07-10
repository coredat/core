#include <core/camera/post_process.hpp>
#include <core/resources/shader.hpp>
#include <core/resources/texture.hpp>
#include <core/resources/render_target.hpp>
#include <data/global_data/post_process_data.hpp>
#include <data/global_data/resource_data.hpp>
#include <common/error_strings.hpp>
#include <utilities/logging.hpp>


namespace Core {


struct Post_process::Impl
{
  uint32_t id;
};


Post_process::Post_process(const char *name)
: m_impl(new Impl{})
{
  auto post_data = Resource_data::get_resources()->post_data;
  
  // Search to see if it exists
  {
    util::generic_id id = util::generic_id_invalid();
  
    Resource_data::data_lock(post_data);
    if(Resource_data::post_process_data_search_property_name(post_data, name))
    {
      m_impl->id = id;
    }
    Resource_data::data_lock(post_data);
    
    if(id)
    {
      return;
    }
  }
  
  // Insert a new
  {
    m_impl->id = post_data->size + 1;
    
    Resource_data::data_lock(post_data);
    
    if(Resource_data::post_process_data_push_back(post_data, m_impl->id))
    {
      Resource_data::post_process_data_set_property_name(post_data, m_impl->id, name);
    }
    else
    {
      assert(false);
      LOG_ERROR(Error_string::no_free_space());
    }
    
    Resource_data::data_unlock(post_data);
  }
}


Post_process::Post_process(Post_process &&other)
: m_impl(new Impl(*other.m_impl.get()))
{
}


Post_process::~Post_process()
{
}


void
Post_process::set_shader(const Core::Shader &core_shader)
{

  auto post_data = Resource_data::get_resources()->post_data;
  auto shd_data = Resource_data::get_resources()->shader_data;
  
  // Update
  { 
    Resource_data::data_lock(shd_data);
    Resource_data::data_lock(post_data);
    
    Ogl::Shader shader;
    Resource_data::shader_data_get_property_shader(shd_data, core_shader.get_id(), &shader);
    
    Post_renderer::Post_shader *post_shd;
    Resource_data::post_process_data_get_property_post_shader(post_data, m_impl->id, &post_shd);
    
    Post_renderer::create_post_shader(post_shd, &shader);
    
    Resource_data::data_unlock(shd_data);
    Resource_data::data_unlock(post_data);
  }
}


void
Post_process::set_input_01(const Texture &core_texture)
{
  auto post_data = Resource_data::get_resources()->post_data;
  auto tex_data = Resource_data::get_resources()->texture_data;
  
  {
    Resource_data::data_lock(post_data);
    Resource_data::data_lock(tex_data);
    
    Post_renderer::Post_shader *post_shd;
    Resource_data::post_process_data_get_property_post_shader(post_data, m_impl->id, &post_shd);
    
    Ogl::Texture texture;
    Resource_data::texture_data_get_property_texture(tex_data, core_texture.get_id(), &texture);
    post_shd->map_01_id = texture;
    
    Resource_data::data_unlock(tex_data);
    Resource_data::data_unlock(post_data);
  }
}


void
Post_process::set_input_02(const Texture &core_texture)
{
  auto post_data = Resource_data::get_resources()->post_data;
  auto tex_data = Resource_data::get_resources()->texture_data;
  
  {
    Resource_data::data_lock(post_data);
    Resource_data::data_lock(tex_data);
    
    Post_renderer::Post_shader *post_shd;
    Resource_data::post_process_data_get_property_post_shader(post_data, m_impl->id, &post_shd);
    
    Ogl::Texture texture;
    Resource_data::texture_data_get_property_texture(tex_data, core_texture.get_id(), &texture);
    post_shd->map_02_id = texture;
    
    Resource_data::data_unlock(tex_data);
    Resource_data::data_unlock(post_data);
  }
}


void
Post_process::set_input_03(const Texture &core_texture)
{
  auto post_data = Resource_data::get_resources()->post_data;
  auto tex_data = Resource_data::get_resources()->texture_data;
  
  {
    Resource_data::data_lock(post_data);
    Resource_data::data_lock(tex_data);
    
    Post_renderer::Post_shader *post_shd;
    Resource_data::post_process_data_get_property_post_shader(post_data, m_impl->id, &post_shd);
    
    Ogl::Texture texture;
    Resource_data::texture_data_get_property_texture(tex_data, core_texture.get_id(), &texture);
    post_shd->map_03_id = texture;
    
    Resource_data::data_unlock(tex_data);
    Resource_data::data_unlock(post_data);
  }
}


uint32_t
Post_process::get_id() const
{
  return m_impl->id;
}


} // ns