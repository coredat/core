#ifndef POST_SHADER_INCLUDED_107F9261_812E_447F_9443_1185574FDA51
#define POST_SHADER_INCLUDED_107F9261_812E_447F_9443_1185574FDA51


#include <graphics_api/ogl/ogl_shader.hpp>
#include <graphics_api/ogl/ogl_shader_uniform.hpp>
#include <graphics_api/ogl/ogl_texture.hpp>


namespace Post_renderer {


struct Post_shader
{
  Ogl::Shader shader;
  
  Ogl::Texture map_01_id;
  Ogl::Uniform map_01;
};


bool
create_post_shader(Post_shader *out_shader, const Ogl::Shader *shader);


} // ns


#endif // inc guard