#include "debug_line_renderer.hpp"
#include <op/op.hpp>
#include <math/general/general.hpp>
#include <utilities/directory.hpp>
#include <utilities/file.hpp>
#include <utilities/string_helpers.hpp>
#include <utilities/logging.hpp>
#include <assert.h>
#include <stddef.h>

/*
  Debug line render
  --
  We double buffer the line data.
  As rendering will be executed elsewhere
  we need to be sure the data still exists.
*/


namespace
{
  constexpr uint32_t number_of_lines = 32;
  constexpr uint32_t number_of_components = 3 * 3; // vec3 * (pos + pos + color)
  
  // We render in batches of 'number_of_lines' so this can be quite big.
  constexpr uint32_t size_of_data_buffer = (1 << 20) * number_of_components;
  
  float line_data_01[size_of_data_buffer];
  float line_data_02[size_of_data_buffer];
  
  size_t line_count_01 = 0;
  size_t line_count_02 = 0;
  
  float *curr_line_data      = nullptr;
  size_t *curr_line_count    = nullptr;
  uint32_t curr_line_counter = 0;
  
  opID line_shader_id;
  opID line_shader_wvp_id;
  opID line_shader_data_id;
  opID line_rasterizer_id;
  
  // Temp Hack
  opContext *temp_ctx;
  opBuffer *temp_buffer;
  
  inline void
  flip_line_data()
  {
    curr_line_data   = curr_line_counter % 2 ? line_data_02 : line_data_01;
    curr_line_count  = curr_line_counter % 2 ? &line_count_02 : &line_count_01;
    *curr_line_count = 0;
    
    ++curr_line_counter;
  }
}


namespace Debug_line_renderer {


void
initialize(opContext *ctx, opBuffer *buf)
{
  LOG_TODO_ONCE("Debug Lines - Heap alloc buffers");
  LOG_TODO_ONCE("Debug Lines - One first camera will have debug lines");
  LOG_TODO_ONCE("Debug Lines - Deal with op hack");

  temp_ctx = ctx;
  temp_buffer = buf;
  
  flip_line_data();

  char debug_lines_shd_path[MAX_FILE_PATH_SIZE];
  {
    memset(debug_lines_shd_path, 0, sizeof(debug_lines_shd_path));
    strcat(debug_lines_shd_path, util::dir::resource_path());
    strcat(debug_lines_shd_path, "assets/shaders/core_debug_line.ogl");
  }
  
  char shader_code[2048];
  {
    memset(shader_code, 0, sizeof(shader_code));
    util::file::get_contents_from_file(debug_lines_shd_path, shader_code, sizeof(shader_code));
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
  
  line_shader_id = opBufferShaderCreate(
    ctx,
    buf,
    "Debug Line Shader",
    vs_code,
    gs_code,
    fs_code,
    &shader_desc
  );
  

  opShaderDataDesc shader_data_desc;
  
  line_shader_wvp_id = opBufferShaderDataCreate(
    ctx,
    buf,
    line_shader_id,
    "uni_wvp_mat",
    &shader_data_desc
  );

  opShaderDataDesc shader_data_line_data_desc;

  line_shader_data_id = opBufferShaderDataCreate(
    ctx,
    buf,
    line_shader_id,
    "uni_line[0]",
    &shader_data_line_data_desc
  );

  opRasterizerDesc line_rasterizer_desc;
  line_rasterizer_desc.cull_face     = opCullFace_BACK;
  line_rasterizer_desc.primitive     = opPrimitive_POINT;
  line_rasterizer_desc.winding_order = opWindingOrder_CCW;
  
  line_rasterizer_id = opBufferRasterizerCreate(
    ctx,
    buf,
    &line_rasterizer_desc
  );
  
  // Build the resources.
  opBufferExec(ctx, buf);
}


void
add_lines(const Line_node nodes[], const std::uint32_t number_of_lines)
{
  for(int32_t i = 0; i < number_of_lines; ++i)
  {
    constexpr uint32_t maximum_lines = size_of_data_buffer / number_of_components;
  
    if(*curr_line_count >= maximum_lines)
    {
      // trying to render more lines than we have capacity for
      assert(false);
      return;
    }
    
    const size_t ptr = *curr_line_count * number_of_components;
    memcpy(&curr_line_data[ptr + 0], &nodes[i], sizeof(float) * number_of_components);
    
    *curr_line_count += 1;
  }
}


void
render(const float wvp_mat[16])
{
  if(*curr_line_count > 0)
  {
    // These args should be sunk into here.
    opBuffer *buf  = temp_buffer;
    opContext *ctx = temp_ctx;

    opBufferDeviceReset(buf);
    opBufferShaderBind(buf, line_shader_id);
    opBufferRasterizerBind(buf, line_rasterizer_id);
    opBufferShaderDataBind(buf, line_shader_wvp_id, (void*)wvp_mat);
    
    uint32_t count = 0;
    
    while(*curr_line_count > 0)
    {
      uint32_t size = math::min(*curr_line_count, number_of_lines);
    
      opBufferShaderDataBind(buf, line_shader_data_id, (void*)&curr_line_data[count * number_of_components]);
      opBufferRenderSubset(buf, 0, size);
      
      count += size;
      *curr_line_count -= size;
    }
    
    opBufferExec(ctx, buf);
    
    // This is double buffered because we woiuld like to defer the render beyond this function if we can.
    flip_line_data();
  }
}

  
} // ns