#include "gui_renderer.hpp"
#include "../graphics_api/texture_filtering.hpp"
#include "../graphics_api/ogl/ogl_texture_filtering.hpp"
#include "../graphics_api/ogl/ogl_common.hpp"
#include "../graphics_api/ogl/ogl_shader.hpp"
#include "../graphics_api/ogl/ogl_shader_uniform.hpp"
#include "../graphics_api/ogl/ogl_texture.hpp"
#include "../graphics_api/ogl/ogl_vertex_format.hpp"
#include "../graphics_api/ogl/ogl_vertex_buffer.hpp"
#include "../graphics_api/utils/shader_utils.hpp"
#include <utils/directory.hpp>
#include <string>
#include <assert.h>
#include <iostream>


namespace
{
  Ogl::Shader                       shader_gui;
  Ogl::Vertex_format                vertex_format;
  Ogl::Vertex_buffer                quad_vbo;
  Ogl::Uniform                      uni_wvp_mat;
  Ogl::Uniform                      uni_quad_size;
  Ogl::Uniform                      uni_quad_color;
  Ogl::Uniform                      uni_diffuse_map;
  Graphics_api::Texture_filtering   texture_filtering;
}


namespace Gui_renderer {


void
initialize()
{
  Ogl::error_clear();

  // Load shader
  {
    const std::string asset_path  = util::get_resource_path() + "assets/";
    const std::string fullbright  = asset_path + "shaders/basic_fullbright.ogl";
    
    const auto gui_code = Graphics_api::Util::shader_code_from_tagged_file(fullbright.c_str());
    
    Ogl::shader_create(&shader_gui, gui_code.vs_code.c_str(), gui_code.gs_code.c_str(), gui_code.ps_code.c_str(), &std::cout);
    
    assert(Ogl::shader_is_valid(shader_gui));
  }
  
  // Get the shader's uniforms
  if(Ogl::shader_is_valid(shader_gui))
  {
    Ogl::Shader_uniforms gui_uniforms;
    Ogl::shader_uniforms_retrive(&gui_uniforms, &shader_gui);
    
    Ogl::shader_uniforms_get_uniform_index(&uni_wvp_mat,     &gui_uniforms, "uni_wvp_mat");
    Ogl::shader_uniforms_get_uniform_index(&uni_quad_size,   &gui_uniforms, "uni_quad_size");
    Ogl::shader_uniforms_get_uniform_index(&uni_quad_color,  &gui_uniforms, "uni_quad_color");
    Ogl::shader_uniforms_get_uniform_index(&uni_diffuse_map, &gui_uniforms, "uni_diffuse_map");
  }
  
  // Vertex attr
  {
    constexpr uint32_t number_of_attrs = 1;
    Ogl::Attribute_desc vert_desc[number_of_attrs]
    {
      Ogl::Attribute_desc{"in_vs_position",       Ogl::Attr_type::FLOAT3},
      //Ogl::Attribute_desc{"in_vs_texture_coord",  Ogl::Attr_type::FLOAT2},
    };
    
    Ogl::vertex_format_load(&vertex_format, vert_desc, number_of_attrs);
    assert(Ogl::vertex_format_is_valid(&vertex_format));
  }
  
  // Build VBO.
  {
    float quad_data[]
    {
      +0.0f, +0.5f, 0.f, //0.f, 0.f,
      +0.5f, -0.5f, 0.f, //1.f,// 0.f,
      -0.5f, -0.5f, 0.f, //0.f,// 1.f,
//      
//      +1.f, +1.f, 1.f, 0.f,
//      +1.f, -1.f, 1.f, 1.f,
//      -1.f, -1.f, 0.f, 1.f,
    };
    
    Ogl::vertex_buffer_load(&quad_vbo, quad_data, sizeof(quad_data), 9 /*???*/, false);
    assert(Ogl::vertex_buffer_is_valid(quad_vbo));
  }
  
  // Default texture filtering
  {
    texture_filtering.wrap_mode_s = Graphics_api::Wrap_mode::clamp;
    texture_filtering.wrap_mode_t = Graphics_api::Wrap_mode::clamp;
    texture_filtering.filtering   = Graphics_api::Filtering_mode::bilinear;
  }
  
  Ogl::error_check("Setting up simple renderers", &std::cout);
}


void
de_initialize()
{
}


void
render_gui_nodes(const Node nodes[],
                 const uint32_t number_of_nodes)
{
  Ogl::error_clear();
  Ogl::default_state();
  
  Ogl::shader_bind(&shader_gui);
  
  for(uint32_t n = 0; n < number_of_nodes; ++n)
  {
    const Node *curr_node = &nodes[n];
    assert(curr_node);
    
    Ogl::vertex_buffer_bind(quad_vbo, &vertex_format, &shader_gui);
    
    //const GLsizei count = quad_vbo.number_of_entries / vertex_format.number_of_attributes;
    glDrawArrays(GL_TRIANGLES, 0, 3);
  }
  
  Ogl::error_check("Drawing the gui", &std::cout);
}


} // ns