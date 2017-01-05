#include "text_renderer.hpp"
#include <op/op.hpp>
#include <math/mat/mat4.hpp>
#include <utilities/file.hpp>
#include <utilities/logging.hpp>
#include <utilities/string_helpers.hpp>
#include <utilities/directory.hpp>

#include <OpenGL/gl3.h>


namespace {

  opID text_shader;
  opID text_shader_wvp;
  opID text_shader_metrics;
  opID text_shader_details;
  opID text_rasterizer;
  
  // Temp Hack
  opContext *temp_ctx;
  opBuffer *temp_buffer;
}


namespace Text_renderer {


void
initialize(opContext *ctx, opBuffer *buf)
{
  LOG_TODO_ONCE("Debug Lines - Deal with op hack");
  
  temp_ctx = ctx;
  temp_buffer = buf;
  
  char core_text_shd_path[MAX_FILE_PATH_SIZE];
  {
    memset(core_text_shd_path, 0, sizeof(core_text_shd_path));
    strcat(core_text_shd_path, util::dir::resource_path());
    strcat(core_text_shd_path, "assets/shaders/core_text.ogl");
  }
  
  char shader_code[1 << 12];
  {
    memset(shader_code, 0, sizeof(shader_code));
    util::file::get_contents_from_file(core_text_shd_path, shader_code, sizeof(shader_code));
    assert(strlen(shader_code));
  }
  
  char vs_code[1024];
  {
    memset(vs_code, 0, sizeof(vs_code));
    util::get_text_between_tags("/* VERT_SHD */", "/* VERT_SHD */", shader_code, vs_code, sizeof(vs_code));
  }

  char gs_code[1024];
  {
    memset(gs_code, 0, sizeof(gs_code));
    util::get_text_between_tags("/* GEO_SHD */", "/* GEO_SHD */", shader_code, gs_code, sizeof(gs_code));
  }
  
  char fs_code[1024];
  {
    memset(fs_code, 0, sizeof(fs_code));
    util::get_text_between_tags("/* FRAG_SHD */", "/* FRAG_SHD */", shader_code, fs_code, sizeof(fs_code));
  }
  
  opShaderDesc shader_desc;
  
  text_shader = opBufferShaderCreate(
    ctx,
    buf,
    "Text Shader",
    vs_code,
    gs_code,
    fs_code,
    &shader_desc
  );
  
  opShaderDataDesc shader_data_wvp_desc;
  
  text_shader_wvp = opBufferShaderDataCreate
  (
    ctx,
    buf,
    text_shader,
    "uni_wvp_mat",
    &shader_data_wvp_desc
  );
  
  opShaderDataDesc shader_data_metrics_desc;
  
  text_shader_metrics = opBufferShaderDataCreate
  (
    ctx,
    buf,
    text_shader,
    "uni_metrics",
    &shader_data_metrics_desc
  );
  
  opShaderDataDesc shader_data_details_desc;
  
  text_shader_details = opBufferShaderDataCreate
  (
    ctx,
    buf,
    text_shader,
    "uni_string",
    &shader_data_details_desc
  );
  
  opRasterizerDesc line_rasterizer_desc;
  line_rasterizer_desc.cull_face     = opCullFace_BACK;
  line_rasterizer_desc.primitive     = opPrimitive_POINT;
  line_rasterizer_desc.winding_order = opWindingOrder_CCW;
  
  text_rasterizer = opBufferRasterizerCreate(
    ctx,
    buf,
    &line_rasterizer_desc
  );
  
  opBufferExec(ctx, buf);
}

void
reset()
{
}


uint32_t
render(const math::mat4 &view_proj_mat,
       const uint32_t cull_mask,
       const Draw_call calls[],
       const uint32_t number_of_calls,
       opContext *ctx,
       opBuffer *buf)
{
  uint32_t draw_call_count = 0;
  
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  
  opBufferRasterizerBind(buf, text_rasterizer);
  opBufferShaderBind(buf, text_shader);
  
  for(uint32_t i = 0; i < number_of_calls; ++i)
  {
    // These need to be textures.
    const auto metrics_id = calls[i].glyph_metrics;
    opBufferShaderDataBind(buf, text_shader_metrics, metrics_id);

    const auto string_id = calls[i].string_info;
    opBufferShaderDataBind(buf, text_shader_details, string_id);
    
    const math::mat4 world_mat = math::mat4_init_with_array(calls[i].world_matrix);
    const math::mat4 wvp_mat = math::mat4_multiply(world_mat, view_proj_mat);
    
    opBufferShaderDataBind(buf, text_shader_wvp, (void*)&wvp_mat);
    opBufferRenderSubset(buf, 0, calls[i].string_size);
    
    ++draw_call_count;
  }
  
  return draw_call_count;
}


} // ns
