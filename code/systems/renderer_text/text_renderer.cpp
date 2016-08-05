#include "text_renderer.hpp"
#include <graphics_api/ogl/ogl_shader.hpp>
#include <graphics_api/ogl/ogl_shader_uniform.hpp>


namespace {

  Ogl::Shader text_shader;

}


namespace Text_renderer {


void
initialize()
{
  const char *vs_shader = R"(
    #version 150 core

    in vec3 vs_in_position;
    in vec3 vs_in_normal;
    in vec2 vs_in_texture_coords;

    uniform mat4 uni_wvp_mat;
  
    out vec2 ps_in_texture_coords;

    void
    main()
    {
      gl_Position = uni_wvp_mat * vec4(vs_in_position, 1.0);
    }
  )";
  
  const char *ps_shader = R"(
    #version 150 core

    in vec2 in_ps_texture_coord;
  
    uniform sampler2D uni_map_01;
  
    out vec4 out_frag_color;
  
    void
    main()
    {
      vec4 tex_sample = texture(uni_map_01, in_ps_texture_coord);
      out_frag_color = tex_sample.rgba;
    }
  )";
}

void
reset()
{
}


uint32_t
render(const math::mat4 &view_proj_mat,
       const Draw_call calls[],
       const uint32_t number_of_calls)
{
  

  return 0;
}


} // ns
