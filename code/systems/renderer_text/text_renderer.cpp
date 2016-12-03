#include "text_renderer.hpp"
#include <graphics_api/ogl/ogl_shader.hpp>
#include <graphics_api/ogl/ogl_shader_uniform.hpp>
#include <graphics_api/ogl/ogl_vertex_format.hpp>
#include <graphics_api/vertex_format.hpp>
#include <graphics_api/texture_filtering.hpp>
#include <graphics_api/ogl/ogl_texture_filtering.hpp>
#include <systems/renderer_common/vertex_format.hpp>
#include <math/mat/mat4.hpp>


namespace {

  Ogl::Shader text_shader;
  Graphics_api::Vertex_format vert_fmt;
  Ogl::Uniform wvp_uni;
  Ogl::Uniform texture_uni;
  Ogl::Uniform texture_metrics;
  Ogl::Uniform string_details;
}


namespace Text_renderer {


void
initialize()
{
  const char *vs_shader = R"(
    #version 330 core

    in vec3 in_vs_position;
    in vec2 in_vs_texture_coord;
    in vec3 in_vs_normal;

    uniform mat4 uni_wvp_mat;
    uniform sampler1D uni_metric_index;
    uniform sampler1D uni_string_detail;
  
    out vec2 in_ps_texture_coord;

    void
    main()
    {
      int data_chunks = 3;
      int char_id = 0; // (0 / 4) * data_chunks * 4;
      
      int str_id = (gl_VertexID / 4);
      
      char_id = str_id;
      
      vec4 chunk_01 = texelFetch(uni_metric_index, char_id, 0);
      vec4 chunk_02 = texelFetch(uni_metric_index, char_id + 1, 0);
      vec4 chunk_03 = texelFetch(uni_metric_index, char_id + 2, 0);
      
      vec2 uv       = chunk_01.xy;
      vec2 st       = chunk_01.zw;
      vec2 size     = chunk_02.rg;
      vec2 advance  = chunk_02.ba;
      vec2 offset   = chunk_03.rg;
      
      
      vec4 str_detail = texelFetch(uni_string_detail, str_id, 0);
      
      
      vec3 pos = vec3(str_detail.x + in_vs_position.x, str_detail.y + in_vs_position.y, in_vs_position.z);
      
      gl_Position =  uni_wvp_mat * vec4(pos, 1.0);
      

      float u = mix(uv.x, st.x, in_vs_texture_coord.x);
      float v = mix(uv.y, st.y, in_vs_texture_coord.y);
      
      in_ps_texture_coord = vec2(u,v);
    }
  )";
  
  const char *ps_shader = R"(
    #version 330 core

    in vec2 in_ps_texture_coord;
  
    uniform sampler2D uni_map_01;
  
    uniform vec3 uni_color = vec3(1,1,1);
  
    out vec4 out_frag_color;
  
    void
    main()
    {
      vec4 tex_sample = texture(uni_map_01, in_ps_texture_coord);
      out_frag_color = vec4(uni_color, tex_sample.r);
//      out_frag_color  = vec4(in_ps_texture_coord, 1,1);
    }
  )";
  
  Ogl::shader_create(&text_shader, vs_shader, nullptr, ps_shader);
  
  vert_fmt = Renderer_common::get_standard_vertex_format();
  
  Ogl::Shader_uniforms unis;
  Ogl::shader_uniforms_retrive(&unis, &text_shader);
  
  Ogl::shader_uniforms_get_uniform_index(&wvp_uni, &unis, "uni_wvp_mat");
  Ogl::shader_uniforms_get_uniform_index(&texture_uni, &unis, "uni_map_01");
  Ogl::shader_uniforms_get_uniform_index(&texture_metrics, &unis, "uni_metric_index");
  Ogl::shader_uniforms_get_uniform_index(&string_details, &unis, "uni_string_detail");
}

void
reset()
{
}


uint32_t
render(const math::mat4 &view_proj_mat,
       const uint32_t cull_mask,
       const Draw_call calls[],
       const uint32_t number_of_calls)
{
  uint32_t draw_call_count = 0;
  
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  Ogl::shader_bind(&text_shader);
  
  for(uint32_t i = 0; i < number_of_calls; ++i)
  {
    Ogl::vertex_buffer_bind(calls[i].mesh.vbo,
                            &vert_fmt.format,
                            &text_shader);
    
    // Move the filter selection into the material.
    {
      const auto id = calls[i].texture.texture_id;
      Ogl::shader_uniforms_apply(texture_uni, (void*)&id);
      
      static Graphics_api::Texture_filtering filter =
      {
        Graphics_api::Wrap_mode::wrap,
        Graphics_api::Wrap_mode::wrap,
        Graphics_api::Filtering_mode::point,      
      };
      
      Ogl::filtering_apply(filter, Graphics_api::Dimention::two);
    }

    {
      const auto metrics_id = calls[i].glyph_metrics.texture_id;
      Ogl::shader_uniforms_apply(texture_metrics, (void*)&metrics_id);
      
      static Graphics_api::Texture_filtering filter =
      {
        Graphics_api::Wrap_mode::wrap,
        Graphics_api::Wrap_mode::wrap,
        Graphics_api::Filtering_mode::point,      
      };
      
      Ogl::filtering_apply(filter, Graphics_api::Dimention::one);
    }
    
    {
      const auto string_id = calls[i].string_info.texture_id;
      Ogl::shader_uniforms_apply(string_details, (void*)&string_id);
      
      static Graphics_api::Texture_filtering filter =
      {
        Graphics_api::Wrap_mode::wrap,
        Graphics_api::Wrap_mode::wrap,
        Graphics_api::Filtering_mode::point,      
      };
      
      Ogl::filtering_apply(filter, Graphics_api::Dimention::one);
    }
    
    const math::mat4 world_mat = math::mat4_init_with_array(calls[i].world_matrix);
    const math::mat4 wvp_mat = math::mat4_multiply(world_mat, view_proj_mat);
    
    Ogl::shader_uniforms_apply(wvp_uni, (void*)&wvp_mat);

    
    const GLsizei count = calls[i].mesh.vbo.number_of_entries / vert_fmt.format.number_of_attributes;
    glDrawArrays(GL_TRIANGLES, 0, count);
    
    ++draw_call_count;
  }
  
  return draw_call_count;
}


} // ns
