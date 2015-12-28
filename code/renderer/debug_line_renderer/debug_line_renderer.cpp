#include "debug_line_renderer.hpp"
#include <simple_renderer/lazy_include.hpp>
#include <assert.h>


namespace
{
  GLuint line_shader_id           = 0;
  GLuint line_shader_uniform_wvp  = 0;
  GLuint line_shader_texture_id   = 0;
}


namespace Debug_line_renderer {


void
initialize(const std::size_t max_number_of_lines)
{
  const char* vertex_shader = R"(
    #version 150 core

    out int gs_in_vert_id;
  
    void main()
    {
      gs_in_vert_id = gl_VertexID;
    }
  )";
  
  const char* geometry_shader = R"(
    #version 150

    layout (points) in;
    layout (triangle_strip, max_vertices = 2) out;
  
    in int              gs_in_vert_id[];
  
    uniform mat4        uni_wvp_mat;
    uniform sampler2D 	uni_data_lookup;
  
    out vec3            ps_in_color;

    void main()
    {
      for(int i = 0; i < 2; i++)
      {
        // Row and Col of the billboard
        float row = floor(i / 2.0);
        float col = mod(i, 2.0);
        
        int id = gs_in_vert_id[0] + i;

        ps_in_color = vec3(1,1,1);

        // Get data for this character
        vec4 point = texelFetch(uni_data_lookup, ivec2(0, id), 0);
        gl_Position = point;//vec4(point, 1.0);

        EmitVertex();
      }

      EndPrimitive();
    }

  )";
  
  const char* fragment_shader = R"(
    #version 150

    in vec3 ps_in_color;
    out vec4 ps_out_color;

    void main()
    {
      ps_out_color = vec4(ps_in_color, 1.0);
    }
  )";

  renderer::shader shd(vertex_shader, geometry_shader, fragment_shader);
  assert(shd.is_valid());

  // Did it all load ok?
  //assert(line_shader_id && line_shader_uniform_wvp && line_shader_texture_id);
}


void
add_lines(const Line_node nodes[], const std::size_t number_of_lines)
{
}


void
render(const float wvp_mat[16])
{
}


} // ns