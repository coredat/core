#include "simple_renderer.hpp"
#include <graphics_api/texture_filtering.hpp>
#include <graphics_api/ogl/ogl_texture_filtering.hpp>
#include <graphics_api/ogl/ogl_common.hpp>
#include <graphics_api/ogl/ogl_shader.hpp>
#include <graphics_api/ogl/ogl_shader_uniform.hpp>
#include <graphics_api/ogl/ogl_texture.hpp>
#include <graphics_api/ogl/ogl_vertex_format.hpp>
#include <graphics_api/utils/shader_utils.hpp>
#include <utilities/directory.hpp>
#include <string>
#include <assert.h>
#include <iostream>


namespace
{
  Ogl::Shader shader_fullbright;
  Ogl::Shader shader_dir_light;
  Ogl::Vertex_format vertex_format;

  Ogl::Uniform uni_full_wvp;
  Ogl::Uniform uni_full_diffuse_map;
  
  Ogl::Uniform uni_dir_wvp;
  Ogl::Uniform uni_dir_world;
  Ogl::Uniform uni_dir_light_dir;
  Ogl::Uniform uni_dir_light_color;
  Ogl::Uniform uni_dir_light_amb;
  Ogl::Uniform uni_dir_light_diff;
  Ogl::Uniform uni_dir_diffuse_map;
  
  Graphics_api::Texture_filtering filtering;
}


namespace Simple_renderer {


void
initialize()
{
  Ogl::error_clear();

  // Load shaders
  {
    const std::string asset_path  = util::get_resource_path() + "assets/";
    const std::string fullbright  = asset_path + "shaders/basic_fullbright.ogl";
    const std::string directional = asset_path + "shaders/basic_dir_light.ogl";
   
    const auto full_code = Graphics_api::Util::shader_code_from_tagged_file(fullbright.c_str());
    const auto dir_code  = Graphics_api::Util::shader_code_from_tagged_file(directional.c_str());

    Ogl::shader_create(&shader_fullbright, full_code.vs_code.c_str(), full_code.gs_code.c_str(), full_code.ps_code.c_str());
    Ogl::shader_create(&shader_dir_light, dir_code.vs_code.c_str(), dir_code.gs_code.c_str(), dir_code.ps_code.c_str());
  }

  // Dir light shader uniforms
  if(Ogl::shader_is_valid(shader_dir_light))
  {
    Ogl::Shader_uniforms dir_light_uniforms;
    Ogl::shader_uniforms_retrive(&dir_light_uniforms, &shader_dir_light);
    
    Ogl::shader_uniforms_get_uniform_index(&uni_dir_wvp,          &dir_light_uniforms, "uni_wvp_mat");
    Ogl::shader_uniforms_get_uniform_index(&uni_dir_world,        &dir_light_uniforms, "uni_world_mat");
    Ogl::shader_uniforms_get_uniform_index(&uni_dir_diffuse_map,  &dir_light_uniforms, "uni_diffuse_map");    
    Ogl::shader_uniforms_get_uniform_index(&uni_dir_light_dir,    &dir_light_uniforms, "uni_dir_light.direction");
    Ogl::shader_uniforms_get_uniform_index(&uni_dir_light_color,  &dir_light_uniforms, "uni_dir_light.color");
    Ogl::shader_uniforms_get_uniform_index(&uni_dir_light_amb,    &dir_light_uniforms, "uni_dir_light.ambient");
    Ogl::shader_uniforms_get_uniform_index(&uni_dir_light_diff,   &dir_light_uniforms, "uni_dir_light.diffuse");

    // Setup constants on direction light
    {
      const float color[3]{ 0.8f,0.7f,0.7f };
      const float dir[3]{ -0.707f,-0.707f,-0.707f };
      const float amb = 0.75f;
      const float diff = 0.43f;

      Ogl::shader_bind(&shader_dir_light);
      Ogl::shader_uniforms_apply(uni_dir_light_color, (void*)color);
      Ogl::shader_uniforms_apply(uni_dir_light_dir, (void*)dir);
      Ogl::shader_uniforms_apply(uni_dir_light_amb, (void*)&amb);
      Ogl::shader_uniforms_apply(uni_dir_light_diff, (void*)&diff);
      Ogl::shader_bind(nullptr);
    }
  }

  // Fullbright shader uniforms
  if (Ogl::shader_is_valid(shader_fullbright))
  {
    Ogl::Shader_uniforms fullbright_uniforms;
    Ogl::shader_uniforms_retrive(&fullbright_uniforms, &shader_fullbright);
    
    Ogl::shader_uniforms_get_uniform_index(&uni_full_wvp,         &fullbright_uniforms, "uni_wvp_mat");
    Ogl::shader_uniforms_get_uniform_index(&uni_full_diffuse_map, &fullbright_uniforms, "uni_diffuse_map");
  }
  
  // Vertex attr
  {
    constexpr uint32_t number_of_attrs = 3;
    Ogl::Attribute_desc vert_desc[number_of_attrs]
    {
      Ogl::Attribute_desc{"in_vs_position",       Ogl::Attr_type::FLOAT3},
      Ogl::Attribute_desc{"in_vs_texture_coord",  Ogl::Attr_type::FLOAT2},
      Ogl::Attribute_desc{"in_vs_normal",         Ogl::Attr_type::FLOAT3},
    };
    
    Ogl::vertex_format_load(&vertex_format, vert_desc, number_of_attrs);
  }
  
  // Default texture filtering
  {
    filtering.wrap_mode_s = Graphics_api::Wrap_mode::clamp;
    filtering.wrap_mode_t = Graphics_api::Wrap_mode::clamp;
    filtering.filtering   = Graphics_api::Filtering_mode::anisotropic;
  }
  
  Ogl::error_check("Setting up simple renderers", &std::cout);
}


void
render_nodes_fullbright(const Node nodes[],
                        const uint32_t number_of_nodes)
{
  Ogl::error_clear();
  Ogl::default_state();
  
  Ogl::shader_bind(&shader_fullbright);
  
  for(uint32_t n = 0; n < number_of_nodes; ++n)
  {
    // Render node.
    const Node *curr_node = &nodes[n];
    assert(curr_node);
    
    if(curr_node->vbo.vertex_buffer_id)
    {
      Ogl::filtering_apply(filtering);
      Ogl::shader_uniforms_apply(uni_full_wvp, (void*)&curr_node->wvp);
      
      // Smplisit state change check.
      {
        static GLuint tex_id = (-1);
        
        if(tex_id != curr_node->diffuse.texture_id)
        {
          tex_id = curr_node->diffuse.texture_id;
          Ogl::shader_uniforms_apply(uni_full_diffuse_map, (void*)&curr_node->diffuse.texture_id);
        }
      }
      
      
      // Simplistic state change check.
      {
        static GLuint last_vbo(-1);
        
        if(last_vbo != curr_node->vbo.vertex_buffer_id)
        {
          last_vbo = curr_node->vbo.vertex_buffer_id;
          Ogl::vertex_buffer_bind(curr_node->vbo, &vertex_format, &shader_fullbright);
        }
      }
      
      const GLsizei count = curr_node->vbo.number_of_entries / vertex_format.number_of_attributes;

      glDrawArrays(GL_TRIANGLES, 0, count);
    }
  }
  
  Ogl::error_check("Drawing fullbright nodes", &std::cout);
}


void
render_nodes_directional_light(const Node nodes[],
                               const uint32_t number_of_nodes)
{
  Ogl::default_state();
  Ogl::shader_bind(&shader_dir_light);
  
  for(uint32_t n = 0; n < number_of_nodes; ++n)
  {
    // Render node.
    const Node *curr_node = &nodes[n];
    assert(curr_node);
    
    Ogl::shader_uniforms_apply(uni_dir_wvp, (void*)&curr_node->wvp);
    Ogl::shader_uniforms_apply(uni_dir_world, (void*)&curr_node->world_mat);
    Ogl::error_check("Setting directional light uniforms", &std::cout);
    Ogl::shader_uniforms_apply(uni_dir_diffuse_map, (void*)&curr_node->diffuse.texture_id);
    Ogl::filtering_apply(filtering);
    Ogl::vertex_buffer_bind(curr_node->vbo, &vertex_format, &shader_dir_light);
    
    const GLsizei count = curr_node->vbo.number_of_entries / vertex_format.number_of_attributes;
    
    glDrawArrays(GL_TRIANGLES, 0, count);
  }
  
  Ogl::error_check("Drawing dir light nodes", &std::cout);  
}


} // ns