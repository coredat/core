#include "debug_line_renderer.hpp"
#include <simple_renderer/lazy_include.hpp>
#include "../low_level_renderer/ogl/ogl_common.hpp"
#include <assert.h>
#include <array>
#include <iostream>
#include "../low_level_renderer/ogl/ogl_texture.hpp"
#include "../low_level_renderer/ogl/ogl_shader.hpp"
#include "../low_level_renderer/ogl/ogl_pixel_format.hpp"
#include <algorithm>


namespace
{
  Ogl::Shader debug_line_shader;
  Ogl::Texture data_texture;
  
  GLint uni_wvp;
  GLint uni_data;
  
  std::vector<float> data;
  std::size_t data_ptr = 0;
}


namespace Debug_line_renderer {


void
initialize()
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
      int row = gs_in_vert_id[0] / 1024;
    
      
      int col = (gs_in_vert_id[0] % 1024) * 4;
    
      vec4 start_point  = texelFetch(uni_data_lookup, ivec2(col + 0, row), 0);
      vec4 end_point    = texelFetch(uni_data_lookup, ivec2(col + 1, row), 0);
      vec4 color        = texelFetch(uni_data_lookup, ivec2(col + 2, row), 0);
      //vec4 padding      = texelFetch(uni_data_lookup, ivec2(col + 4, row), 0);
      

      ps_in_color = vec3(color.xyz);

      gl_Position = uni_wvp_mat * vec4(start_point);
      EmitVertex();
      
      gl_Position = uni_wvp_mat * vec4(end_point);
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
  
  Ogl::shader_create(&debug_line_shader, vertex_shader, geometry_shader, fragment_shader, &std::cout);
  Ogl::error_check("Building debug line shader.", &std::cout);

  // Size the data container.
  {
    const std::size_t width_of_data         = 1024;
    const std::size_t height_of_data        = width_of_data;
    const std::size_t number_of_components  = 4;
    
    data.resize((width_of_data * height_of_data) * number_of_components, 0);
  
    Ogl::texture_create_2d(&data_texture, width_of_data, height_of_data, GL_RGBA32F, (void*)data.data(), &std::cout);
  }
  
  uni_wvp  = glGetUniformLocation(debug_line_shader.program_id, "uni_wvp_mat");
  uni_data = glGetUniformLocation(debug_line_shader.program_id, "uni_data_lookup");
  
  Ogl::error_check("Debug Renderer Setup.", &std::cout);
}


void
add_lines(const Line_node nodes[], const std::size_t number_of_lines)
{
  // Shit - can do two memcpy's instead
  for(int32_t i = 0; i < number_of_lines; ++i)
  {
    const std::size_t start_index = (data_ptr * (16)) % data.size();
    
    data.at(start_index + 0) = nodes[i].position_from[0];
    data.at(start_index + 1) = nodes[i].position_from[1];
    data.at(start_index + 2) = nodes[i].position_from[2];
    data.at(start_index + 3) = 1;
    
    data.at(start_index + 4) = nodes[i].position_to[0];
    data.at(start_index + 5) = nodes[i].position_to[1];
    data.at(start_index + 6) = nodes[i].position_to[2];
    data.at(start_index + 7) = 1;
    
    data.at(start_index + 8)  = nodes[i].color[0];
    data.at(start_index + 9)  = nodes[i].color[1];
    data.at(start_index + 10) = nodes[i].color[2];
    data.at(start_index + 11) = 1;
    
    data_ptr++;
  }
}


void
render(const float wvp_mat[16])
{
  // Update texture
  Ogl::texture_update_texture_2d(&data_texture, 0, 0, data_texture.width, data_texture.height, (void*)data.data());
  Ogl::error_check("Updating texture", &std::cout);
  
  // Render
  Ogl::reset_state();
  
  glUseProgram(debug_line_shader.program_id);
  Ogl::error_check("Use program", &std::cout);
  
  glUniformMatrix4fv(uni_wvp, 1, GL_FALSE, wvp_mat);
  Ogl::error_check("set wvp.", &std::cout);
  
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, data_texture.texture_id);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  
  glDrawArrays(GL_POINTS, 0, static_cast<GLsizei>(data_ptr));
  data_ptr = 0;

  Ogl::error_check("Debug line renderer.", &std::cout);
}


} // ns