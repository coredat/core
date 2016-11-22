#include <systems/renderer_material/material_renderer.hpp>
#include <graphics_api/texture_filtering.hpp>
#include <graphics_api/ogl/ogl_texture_filtering.hpp>
#include <graphics_api/ogl/ogl_common.hpp>
#include <graphics_api/ogl/ogl_shader.hpp>
#include <graphics_api/ogl/ogl_shader_uniform.hpp>
#include <graphics_api/ogl/ogl_texture.hpp>
#include <graphics_api/ogl/ogl_vertex_format.hpp>
#include <graphics_api/utils/shader_utils.hpp>
#include <math/mat/mat4.hpp>


namespace
{
  GLuint mat_renderer_last_program        = -1;
  GLuint mat_renderer_last_ibo            = -1;
  GLuint mat_renderer_last_vbo            = -1;
  GLuint mat_renderer_last_map_01_texture = -1;
  
  Ogl::Vertex_format mat_renderer_vertex_format;
}


namespace Material_renderer {


void
initialize()
{
  // Vertex attr
  {
    constexpr uint32_t number_of_attrs = 3;
    const Ogl::Attribute_desc vert_desc[number_of_attrs]
    {
      Ogl::Attribute_desc{"in_vs_position",       Ogl::Attr_type::FLOAT3},
      Ogl::Attribute_desc{"in_vs_texture_coord",  Ogl::Attr_type::FLOAT2},
      Ogl::Attribute_desc{"in_vs_normal",         Ogl::Attr_type::FLOAT3},
    };
    
    Ogl::vertex_format_load(&mat_renderer_vertex_format,
                            vert_desc,
                            number_of_attrs);
  }
}


void
reset()
{
  mat_renderer_last_ibo             = -1;
  mat_renderer_last_vbo             = -1;
  mat_renderer_last_program         = -1;
  mat_renderer_last_map_01_texture  = -1;
  
  Ogl::default_state();
}


uint32_t
render(const math::mat4 &view_proj_mat,
       const math::vec3 eye_pos,
       const float delta_time,
       const float total_time,
       const Material *material,
       const uint32_t cull_mask,
       const Draw_call calls[],
       const uint32_t number_of_calls,
       Ogl::Texture light_texture)
{
  uint32_t draw_calls_count = 0;

  Ogl::error_clear();
  
  // TODO: This is a hack! for text
  LOG_TODO_ONCE("Blending hack");
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  
  if(material->shader.program_id == 0)
  {
    LOG_WARNING("Rendering zero program?");
    return draw_calls_count;
  }

  // Bind material.
  {
    // Bind the program unless it is already bound.
    if(material->shader.program_id != mat_renderer_last_program)
    {
      Ogl::shader_bind(&material->shader);
      mat_renderer_last_program = material->shader.program_id;
    }
    
    // Texture
    if(material->map_01.texture_id != mat_renderer_last_map_01_texture)
    {
      if(material->map_01_index.index != -1)
      {
        // Move the filter selection into the material.
        static Graphics_api::Texture_filtering filter =
        {
          Graphics_api::Filtering_mode::anisotropic,
          Graphics_api::Wrap_mode::wrap,
          Graphics_api::Wrap_mode::wrap
        };
        
        Ogl::filtering_apply(filter, Graphics_api::Dimention::two);
        Ogl::shader_uniforms_apply(material->map_01_index, (void*)&material->map_01.texture_id);
      }
      
      mat_renderer_last_map_01_texture = material->map_01.texture_id;
    }
    
    // Color
    if(material->color.index >= 0)
    {
      Ogl::shader_uniforms_apply(material->color, (void*)material->color_data);
    }
    
    // Lighting
    {
      Ogl::shader_uniforms_apply(material->vec3_eye_position, (void*)&eye_pos);
      
      // Move the filter selection into the material.
      static Graphics_api::Texture_filtering filter =
      {
        Graphics_api::Filtering_mode::bilinear,
        Graphics_api::Wrap_mode::clamp,
        Graphics_api::Wrap_mode::clamp
      };
      
      Ogl::filtering_apply(filter, Graphics_api::Dimention::one);
      Ogl::shader_uniforms_apply(material->light_array, (void*)&light_texture.texture_id);
    }
    
    // Other stuff
    {
      if(material->uni_dt.index >= 0)
      {
        Ogl::shader_uniforms_apply(material->uni_dt, (void*)&delta_time);
      }
      
      if(material->uni_total_time.index >= 0)
      {
        Ogl::shader_uniforms_apply(material->uni_total_time, (void*)&total_time);
      }
    }
    
    const GLenum err_code = glGetError();
    if(err_code != GL_NO_ERROR)
    {
      LOG_GL_ERROR(err_code, "Setting up material for run");
    }
  }
  
  // Draw all the vbo's
  for(uint32_t i = 0; i < number_of_calls; ++i)
  {
    const Draw_call &call = calls[i];
    
    if(cull_mask != UINT32_MAX && (cull_mask & call.cull_mask) == 0)
    {
      continue;
    }
    
    // Per Mesh Mats
    {
      // WVP
      if(material->mat_world_view_proj.index >= 0)
      {
        const math::mat4 *world = reinterpret_cast<const math::mat4*>(&call.world_matrix[0]);
        math::mat4 wvp = math::mat4_multiply(*world, view_proj_mat);
        Ogl::shader_uniforms_apply(material->mat_world_view_proj, (void*)&wvp);
      }
      
      // World
      if(material->mat_world.index >= 0)
      {
        Ogl::shader_uniforms_apply(material->mat_world, (void*)&call.world_matrix);
      }
    }

    // VBO.
    if(mat_renderer_last_vbo != call.mesh.vbo.vertex_buffer_id)
    {
      Ogl::vertex_buffer_bind(call.mesh.vbo, &mat_renderer_vertex_format, &material->shader);
      mat_renderer_last_vbo = call.mesh.vbo.vertex_buffer_id;
    }
    
    // IBO
    if(mat_renderer_last_ibo != call.mesh.ibo.index_buffer_id)
    {
      Ogl::index_buffer_bind(call.mesh.ibo);
      ++draw_calls_count;
    }
    
    // Draw the mesh with ibo if we have one.
    if(call.mesh.ibo.index_buffer_id)
    {
      Ogl::index_buffer_draw(GL_TRIANGLES, call.mesh.ibo);
    }
    // Or vbo if we don't.
    else
    {
      if(mat_renderer_vertex_format.number_of_attributes)
      {
        const GLsizei count = call.mesh.vbo.number_of_entries / mat_renderer_vertex_format.number_of_attributes;
        glDrawArrays(GL_TRIANGLES, 0, count);
        ++draw_calls_count;
      }
    }
  }
  
  const GLenum err_code = glGetError();
  if(err_code != GL_NO_ERROR)
  {
    LOG_GL_ERROR(err_code, "Rendering material");
  }
  
  return draw_calls_count;
}


} // ns