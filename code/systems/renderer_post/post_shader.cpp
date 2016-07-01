#include <systems/renderer_post/post_shader.hpp>
#include <common/error_strings.hpp>


namespace Post_renderer {


bool
create_post_shader(Post_shader *out_shader, const Ogl::Shader *shader)
{
  assert(out_shader && shader);
  assert(Ogl::shader_is_valid(shader));

  if(!Ogl::shader_is_valid(shader))
  {
    LOG_ERROR(Error_string::failed_to_create_resource());
    return false;
  }
  
  out_shader->shader = *shader;

  Ogl::Shader_uniforms uniforms;
  Ogl::shader_uniforms_retrive(&uniforms, shader);
  
  // Texture inputs
  {
    Ogl::shader_uniforms_get_uniform_index(&out_shader->map_01, &uniforms, "uni_map_01");
    
    Ogl::shader_uniforms_get_uniform_index(&out_shader->map_02, &uniforms, "uni_map_02");
    
    Ogl::shader_uniforms_get_uniform_index(&out_shader->map_03, &uniforms, "uni_map_03");
  }
  
  // Other inputs
  {
    Ogl::shader_uniforms_get_uniform_index(&out_shader->delta_time_uni, &uniforms, "uni_delta_time");
    Ogl::shader_uniforms_get_uniform_index(&out_shader->current_time_uni, &uniforms, "uni_current_time");
  }

  return true;
}


} // ns