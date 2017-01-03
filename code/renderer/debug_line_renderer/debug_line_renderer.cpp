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

#include <op/op.hpp>


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
  
  opID line_shader_id;
  opID line_shader_wvp_id;
  opID line_shader_data_id[line_uniform_max];
  opID line_rasterizer_id;
  
  // Temp Hack
  opContext *temp_ctx;
  opBuffer *temp_buffer;
}


namespace Debug_line_renderer {


void
//initialize(Graphics_api::Context *ctx)
initialize(opContext *ctx, opBuffer *buf)
{
  temp_ctx = ctx;
  temp_buffer = buf;

  Ogl::error_clear();

  char debug_lines_shd_path[MAX_FILE_PATH_SIZE];
  memset(debug_lines_shd_path, 0, sizeof(debug_lines_shd_path));
  
  strcat(debug_lines_shd_path, util::dir::resource_path());
  strcat(debug_lines_shd_path, "assets/shaders/debug_line.ogl");
  
  auto debug_code = Graphics_api::Util::shader_code_from_tagged_file(debug_lines_shd_path);
  
  
//  Graphics_api::Shader_desc shd_desc;
//  ctx->shader_create(&shd_desc, debug_lines_shd_path);
  
  opShaderDesc shader_desc;
  
  line_shader_id = opBufferShaderCreate(
    ctx,
    buf,
    "Debug Line Shader",
    debug_code.vs_code.c_str(),
    debug_code.gs_code.c_str(),
    debug_code.ps_code.c_str(),
    &shader_desc
  );
  
//  Ogl::shader_create(&debug_line_shader, debug_code.vs_code.c_str(), debug_code.gs_code.c_str(), debug_code.ps_code.c_str());
//  assert(Ogl::shader_is_valid(&debug_line_shader));
//  
//  debug_line_shader.program_id = shd_desc.platform_handle;

  opShaderDataDesc shader_data_desc;
  
  line_shader_wvp_id = opBufferShaderDataCreate(
    ctx,
    buf,
    line_shader_id,
    "uni_wvp_mat",
    &shader_data_desc
  );
  
  char name_buffer[128];
  
  for(uint32_t i = 0; i < line_uniform_max; ++i)
  {
    opShaderDataDesc shader_data_line_data;
    
    memset(name_buffer, 0, sizeof(name_buffer));
    sprintf(name_buffer, "uni_line[%d]", i);
    
    line_shader_data_id[i] = opBufferShaderDataCreate(
      ctx,
      buf,
      line_shader_id,
      name_buffer,
      &shader_data_line_data
    );
    
    opBufferExec(ctx, buf); // We needed todo this because we can't hold on to  all the uniform names.
  }
  
  opRasterizerDesc line_rasterizer_desc;
  line_rasterizer_desc.cull_face     = opCullFace_NONE;
  line_rasterizer_desc.primitive     = opPrimitive_POINT;
  line_rasterizer_desc.winding_order = opWindingOrder_CCW;
  
  line_rasterizer_id = opBufferRasterizerCreate(
    ctx,
    buf,
    &line_rasterizer_desc
  );
  
  opBufferExec(ctx, buf);

//  if(Ogl::shader_is_valid(&debug_line_shader))
//  {
//    Ogl::Shader_uniforms uniforms;
//    Ogl::shader_uniforms_retrive(&uniforms, &debug_line_shader);
//    Ogl::shader_uniforms_get_uniform_index(&uni_wvp, &uniforms, "uni_wvp_mat");
//  
//    for(uint32_t i = 0; i < line_uniform_max; ++i)
//    {
//      const std::string uni_name = "uni_line[" + std::to_string(i) + "]";
//      uni_line[i] = glGetUniformLocation(debug_line_shader.program_id, uni_name.c_str());
//    }
//  }
  
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
  opBuffer *buf = temp_buffer;
  opContext *ctx = temp_ctx;

  opBufferShaderBind(buf, line_shader_id);
  opBufferRasterizerBind(buf, line_rasterizer_id);
  opBufferShaderDataBind(buf, line_shader_wvp_id, &wvp_mat);

  const uint32_t number_to_batch = (data_ptr / number_of_lines) + 1;
  
  // Render
//  Ogl::default_state();
  
  uint32_t data_get = 0; // counter to how many we have already draw from the buffer.
  
  //glUseProgram(debug_line_shader.program_id);
//  Ogl::shader_bind(&debug_line_shader);
//  Ogl::shader_uniforms_apply(uni_wvp, (void*)wvp_mat);
  
  for(uint32_t b = 0; b < number_to_batch; ++b)
  {
    for(uint32_t l = 0; l < std::min<uint32_t>(number_of_lines, data_ptr); ++l)
    {
      const uint32_t uni = l * number_of_components;
      
      // TODO: If I stored the info in a 3x3 matrix would uploading be quicker?
      opBufferShaderDataBind(buf, line_shader_data_id[uni + 0], &data[data_get + 0]);
      opBufferShaderDataBind(buf, line_shader_data_id[uni + 1], &data[data_get + 3]);
      opBufferShaderDataBind(buf, line_shader_data_id[uni + 2], &data[data_get + 6]);
      
//      glUniform3f(uni_line[uni + 0], data[data_get+ 0], data[data_get+ 1], data[data_get+2]);
//      glUniform3f(uni_line[uni + 1], data[data_get+ 3], data[data_get+ 4], data[data_get+5]);
//      glUniform3f(uni_line[uni + 2], data[data_get+ 6], data[data_get+ 7], data[data_get+8]);
      
      data_get += number_of_components * 3; // * 3 because we are doing 3 at a time.
      data_ptr -= (number_of_components * 3);
    }
    
    
//    glDrawArrays(GL_POINTS, 0, static_cast<GLsizei>(line_uniform_max));

    opBufferRenderSubset(buf, 0, line_uniform_max);
    opBufferExec(ctx, buf);
  }
  
  assert(data_ptr == 0); // We need to have drawn all the lines, if not somethings out of sync.

  const GLenum err_code = glGetError();
  if(err_code != GL_NO_ERROR)
  {
    LOG_GL_ERROR(err_code, "Rendering debug lines");
  }}


} // ns