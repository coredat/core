#include "debug_line_renderer.hpp"
#include <graphics_api/ogl/ogl_common.hpp>
#include <graphics_api/ogl/ogl_texture.hpp>
#include <graphics_api/ogl/ogl_shader.hpp>
#include <graphics_api/ogl/ogl_shader_uniform.hpp>
#include <graphics_api/ogl/ogl_pixel_format.hpp>
#include <graphics_api/pixel_format.hpp>
#include <graphics_api/ogl/ogl_texture_filtering.hpp>
#include <graphics_api/utils/shader_utils.hpp>
#include <utilities/directory.hpp>
#include <utilities/file.hpp>
#include <algorithm>
#include <assert.h>
#include <stddef.h>


#include <graphics_api/vertex_format.hpp>
#include <graphics_api/uniform.hpp>
#include <graphics_api/shader.hpp>

#include <graphics_api/context.hpp>
#include <graphics_api/buffer.hpp>
#include <graphics_api/shader_desc.hpp>


namespace
{
  Ogl::Shader debug_line_shader;
  Ogl::Uniform uni_wvp;
  
  constexpr uint32_t number_of_lines = 32;
  constexpr uint32_t number_of_components = 3;
  constexpr uint32_t line_uniform_max = number_of_lines * number_of_components; // 64 lines, 3 components per line (start, end, color)
  GLint uni_line[line_uniform_max];
  
  constexpr uint32_t size_of_data_buffer = (1 << 20) * number_of_components; // Some big size * number of components (start, end, color)
  float data[size_of_data_buffer];
  
  uint32_t data_ptr = 0; // Keeps a track of the data that we push in.
}


namespace Debug_line_renderer {


void
initialize(Graphics_api::Context *ctx)
{
  Ogl::error_clear();

  char debug_lines_shd_path[MAX_FILE_PATH_SIZE];
  memset(debug_lines_shd_path, 0, sizeof(debug_lines_shd_path));
  
  strcat(debug_lines_shd_path, util::dir::resource_path());
  strcat(debug_lines_shd_path, "assets/shaders/debug_line.ogl");
  
  auto debug_code = Graphics_api::Util::shader_code_from_tagged_file(debug_lines_shd_path);
  
  
  Graphics_api::Shader_desc shd_desc;
  ctx->shader_create(&shd_desc, debug_lines_shd_path);
  
  
  
  Ogl::shader_create(&debug_line_shader, debug_code.vs_code.c_str(), debug_code.gs_code.c_str(), debug_code.ps_code.c_str());
  assert(Ogl::shader_is_valid(&debug_line_shader));
  
  debug_line_shader.program_id = shd_desc.platform_handle;

  if(Ogl::shader_is_valid(&debug_line_shader))
  {
    Ogl::Shader_uniforms uniforms;
    Ogl::shader_uniforms_retrive(&uniforms, &debug_line_shader);
    Ogl::shader_uniforms_get_uniform_index(&uni_wvp, &uniforms, "uni_wvp_mat");
  
    for(uint32_t i = 0; i < line_uniform_max; ++i)
    {
      const std::string uni_name = "uni_line[" + std::to_string(i) + "]";
      uni_line[i] = glGetUniformLocation(debug_line_shader.program_id, uni_name.c_str());
    }
  }
  
  const GLenum err_code = glGetError();
  if(err_code != GL_NO_ERROR)
  {
    LOG_GL_ERROR(err_code, "Setting up debug line renderer");
  }
}


void
add_lines(const Line_node nodes[], const std::uint32_t number_of_lines)
{
  // Shit - can do two memcpy's instead
  for(int32_t i = 0; i < number_of_lines; ++i)
  {
    if(data_ptr >= size_of_data_buffer)
    {
     // assert(false); // trying to render more lines than we have capacity for.
      return;
    }
    
    data[data_ptr++] = nodes[i].position_from[0];
    data[data_ptr++] = nodes[i].position_from[1];
    data[data_ptr++] = nodes[i].position_from[2];
    
    data[data_ptr++] = nodes[i].position_to[0];
    data[data_ptr++] = nodes[i].position_to[1];
    data[data_ptr++] = nodes[i].position_to[2];
    
    data[data_ptr++] = nodes[i].color[0];
    data[data_ptr++] = nodes[i].color[1];
    data[data_ptr++] = nodes[i].color[2];
  }
}


void
render(const float wvp_mat[16])
{
  const uint32_t number_to_batch = (data_ptr / number_of_lines) + 1;
  
  
  // Render
  Ogl::default_state();
  
  uint32_t data_get = 0; // counter to how many we have already draw from the buffer.
  
  //glUseProgram(debug_line_shader.program_id);
  Ogl::shader_bind(&debug_line_shader);
  Ogl::shader_uniforms_apply(uni_wvp, (void*)wvp_mat);
  
  for(uint32_t b = 0; b < number_to_batch; ++b)
  {
    for(uint32_t l = 0; l < std::min<uint32_t>(number_of_lines, data_ptr); ++l)
    {
      const uint32_t uni = l * number_of_components;
      
      // TODO: If I stored the info in a 3x3 matrix would uploading be quicker?
      glUniform3f(uni_line[uni + 0], data[data_get+ 0], data[data_get+ 1], data[data_get+2]);
      glUniform3f(uni_line[uni + 1], data[data_get+ 3], data[data_get+ 4], data[data_get+5]);
      glUniform3f(uni_line[uni + 2], data[data_get+ 6], data[data_get+ 7], data[data_get+8]);
      
      data_get += number_of_components * 3; // * 3 because we are doing 3 at a time.
      data_ptr -= (number_of_components * 3);
    }
    
    
    glDrawArrays(GL_POINTS, 0, static_cast<GLsizei>(line_uniform_max));
  }
  
  assert(data_ptr == 0); // We need to have drawn all the lines, if not somethings out of sync.

  const GLenum err_code = glGetError();
  if(err_code != GL_NO_ERROR)
  {
    LOG_GL_ERROR(err_code, "Rendering debug lines");
  }}


} // ns