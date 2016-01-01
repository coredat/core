#include "debug_line_renderer.hpp"
#include <simple_renderer/lazy_include.hpp>
#include "../low_level_renderer/ogl/ogl_common.hpp"
#include <assert.h>
#include <array>
#include <iostream>
#include "../low_level_renderer/ogl/ogl_texture.hpp"
#include "../low_level_renderer/ogl/ogl_pixel_format.hpp"
#include <algorithm>


namespace
{
  GLuint line_shader_id           = 0;
  GLuint line_shader_uniform_wvp  = 0;
  GLuint line_shader_texture_id   = 0;
  
  renderer::shader shd;
  renderer::texture texture;
  
  GLint uniTrans;
  GLint uni_data;
  
  Ogl::Texture data_texture;
  std::vector<float> data;
  std::size_t data_ptr = 0;
}


namespace Debug_line_renderer {


void
initialize(const std::size_t max_number_of_lines)
{
  Ogl::error_clear();

  const char* vertex_shader = R"(
    #version 150 core

    out int gs_in_vert_id;
  
    void main()
    {
      gs_in_vert_id = gl_VertexID;
    }
  )";
  
  const char* geometry_shader = R"(
    #version 330

    layout (points) in;
    layout (line_strip, max_vertices = 2) out;
  
    in int              gs_in_vert_id[];
  
    uniform mat4        uni_wvp_mat;
    uniform sampler2D 	uni_data_lookup;
  
    out vec3            ps_in_color;

    void main()
    {
      vec4 data_chunk_1 = texelFetch(uni_data_lookup, ivec2(0, gs_in_vert_id[0]), 0);
      vec4 data_chunk_2 = texelFetch(uni_data_lookup, ivec2(1, gs_in_vert_id[0]), 0);
      vec4 data_chunk_3 = texelFetch(uni_data_lookup, ivec2(2, gs_in_vert_id[0]), 0);

      ps_in_color = vec3(data_chunk_3.xyz);

      gl_Position = uni_wvp_mat * vec4(data_chunk_1.xyz, 1.0);
      EmitVertex();
      
      gl_Position = uni_wvp_mat * vec4(data_chunk_2.xyz, 1.0);
      EmitVertex();
      
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

  shd.load_shader(renderer::shader_utils::build_shader_code_vs_gs_ps(vertex_shader, geometry_shader, fragment_shader));
  assert(shd.is_valid());

  // Did it all load ok?
  //assert(line_shader_id && line_shader_uniform_wvp && line_shader_texture_id);
  
  data.resize((16 * 1024) * 4, 0);
  
  Ogl::texture_create_2d(&data_texture, 16, 1024, Ogl::Pixel_format::rgba32f, (void*)data.data(), &std::cout);
  
  uniTrans = glGetUniformLocation(shd.get_program_gl_id(), "uni_wvp_mat");
  uni_data = glGetUniformLocation(shd.get_program_gl_id(), "uni_data_lookup");
  
  Ogl::error_check(&std::cout, "Debug Renderer Setup.");
}


void
add_lines(const Line_node nodes[], const std::size_t number_of_lines)
{
  // Shit - can do two memcpy's instead
  for(int32_t i = 0; i < number_of_lines; ++i)
  {
    data_ptr = data_ptr % 1024;
    
    const std::size_t start_index = data_ptr * (16 * 4);
    
    data.at(start_index + 0) = nodes[i].position_from[0];
    data.at(start_index + 1) = nodes[i].position_from[1];
    data.at(start_index + 2) = nodes[i].position_from[2];
    data.at(start_index + 3) = 0;
    
    data.at(start_index + 4) = nodes[i].position_to[0];
    data.at(start_index + 5) = nodes[i].position_to[1];
    data.at(start_index + 6) = nodes[i].position_to[2];
    data.at(start_index + 7) = 0;
    
    data.at(start_index + 8) = nodes[i].color[0];
    data.at(start_index + 9) = nodes[i].color[1];
    data.at(start_index + 10) = nodes[i].color[2];
    data.at(start_index + 11) = 0;
    
    data_ptr++;
  }
}


void
render(const float wvp_mat[16])
{
  // Update texture
  Ogl::texture_update_texture_2d(&data_texture, 0, 0, 16, 1024, (void*)data.data());
  Ogl::error_check(&std::cout, "Updating texture");

  renderer::reset();
  
  glEnable(GL_PROGRAM_POINT_SIZE);
  glPointSize(5.0);
  glDisable(GL_CULL_FACE);
  
  glUseProgram(shd.get_program_gl_id());

  Ogl::error_check(&std::cout, "Use program");
  
  glUniformMatrix4fv(uniTrans, 1, GL_FALSE, wvp_mat);
  
  Ogl::error_check(&std::cout, "set wvp.");
  
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, data_texture.texture_id);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  
  glDrawArrays(GL_POINTS, 0, data_ptr);
  data_ptr = 0;

  
  Ogl::error_check(&std::cout, "Debug line renderer.");
}


} // ns