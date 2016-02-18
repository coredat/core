#include "simple_renderer.hpp"
#include "../graphics_api/texture_filtering.hpp"
#include "../graphics_api/ogl/ogl_texture_filtering.hpp"
#include "../graphics_api/ogl/ogl_shader.hpp"
#include "../graphics_api/ogl/ogl_shader_uniform.hpp"
#include "../graphics_api/ogl/ogl_common.hpp"
#include "../graphics_api/ogl/ogl_texture.hpp"
#include "../graphics_api/ogl/ogl_vertex_format.hpp"
#include <simple_renderer/lazy_include.hpp> // Used to parse the shader code. Boo!
#include <utils/directory.hpp>
#include <string>
#include <assert.h>
#include <iostream>


namespace
{
  Ogl::Shader shader_fullbright;
  Ogl::Shader shader_dir_light;
  Ogl::Vertex_format vertex_format;

  GLint uni_full_wvp(0);
  GLint uni_dir_wvp(0);
  GLint uni_dir_world(0);
  GLint uni_dir_light_dir(0);
  GLint uni_dir_light_color(0);
  GLint uni_dir_light_amb(0);
  GLint uni_dir_light_diff(0);
  
  Graphics_api::Texture_filtering filtering;
}


namespace Simple_renderer {


void
initialize()
{
  Ogl::error_clear();

  const std::string asset_path = util::get_resource_path() + "assets/";
  const auto full_code = renderer::shader_utils::get_shader_code_from_tagged_file(asset_path + "shaders/basic_fullbright.ogl");
  const auto dir_code  = renderer::shader_utils::get_shader_code_from_tagged_file(asset_path + "shaders/basic_dir_light.ogl");

  Ogl::shader_create(&shader_fullbright, full_code.vs_code.c_str(), full_code.gs_code.c_str(), full_code.ps_code.c_str(), &std::cout);
  Ogl::shader_create(&shader_dir_light, dir_code.vs_code.c_str(), dir_code.gs_code.c_str(), dir_code.ps_code.c_str(), &std::cout);

  Ogl::Shader_uniforms dir_light_uniforms;
  Ogl::shader_uniforms_retrive(&dir_light_uniforms, &shader_dir_light);
  
  Ogl::shader_uniforms_get_uniform_index(&uni_dir_wvp,          &dir_light_uniforms, "wvp");
  Ogl::shader_uniforms_get_uniform_index(&uni_dir_world,        &dir_light_uniforms, "world");
  Ogl::shader_uniforms_get_uniform_index(&uni_dir_light_dir,    &dir_light_uniforms, "dir_light.direction");
  Ogl::shader_uniforms_get_uniform_index(&uni_dir_light_color,  &dir_light_uniforms, "dir_light.color");
  Ogl::shader_uniforms_get_uniform_index(&uni_dir_light_amb,    &dir_light_uniforms, "dir_light.ambient");
  Ogl::shader_uniforms_get_uniform_index(&uni_dir_light_diff,   &dir_light_uniforms, "dir_light.diffuse");
  
  Ogl::Shader_uniforms fullbright_uniforms;
  Ogl::shader_uniforms_retrive(&dir_light_uniforms, &shader_fullbright);
  
  Ogl::shader_uniforms_get_uniform_index(&uni_full_wvp, &fullbright_uniforms, "wvp");
  
  constexpr size_t number_of_attrs = 3;
  Ogl::Attribute_desc vert_desc[number_of_attrs]
  {
      Ogl::Attribute_desc{"in_vs_position",       Ogl::Attr_type::FLOAT3},
      Ogl::Attribute_desc{"in_vs_texture_coord",  Ogl::Attr_type::FLOAT2},
      Ogl::Attribute_desc{"in_vs_normal",         Ogl::Attr_type::FLOAT3},
  };
  
  Ogl::vertex_format_load(&vertex_format, vert_desc, number_of_attrs);
  
  filtering.wrap_mode_s = Graphics_api::Wrap_mode::clamp;
  filtering.wrap_mode_t = Graphics_api::Wrap_mode::clamp;
  filtering.filtering   = Graphics_api::Filtering_mode::anisotropic;
  
  Ogl::shader_bind(&shader_dir_light);
  
  const float color[3] {0.8f,0.7f,0.7f};
  glUniform3f(uni_dir_light_color, color[0], color[1], color[2]);
  
  const float dir[3] {-0.707f,-0.707f,-0.707f};
  glUniform3f(uni_dir_light_dir, dir[0], dir[1], dir[2]);

  const float amb = 0.75f;
  glUniform1f(uni_dir_light_amb, amb);
  
  const float diff = 0.8f;
  glUniform1f(uni_dir_light_diff, diff);
  Ogl::error_check("Setting light uniforms", &std::cout);  
  
  Ogl::error_check("Setting up simple renderers", &std::cout);
}


void
render_nodes_fullbright(const Node nodes[],
                        const size_t number_of_nodes)
{
  Ogl::default_state();
  Ogl::shader_bind(&shader_fullbright);
  
  for(size_t n = 0; n < number_of_nodes; ++n)
  {
    // Render node.
    const Node *curr_node = &nodes[n];
    assert(curr_node);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, curr_node->diffuse_id);
    
    Ogl::filtering_apply(filtering);
    Ogl::vertex_buffer_bind(curr_node->vbo, &vertex_format, &shader_fullbright);

    glUniformMatrix4fv(uni_full_wvp, 1, GL_FALSE, curr_node->wvp);

    const GLsizei count = curr_node->vbo.number_of_entries / vertex_format.number_of_attributes;

    glDrawArrays(GL_TRIANGLES, 0, count);
  }
  
  Ogl::error_check("Drawing fullbright nodes", &std::cout);
}


void
render_nodes_directional_light(const Node nodes[], const size_t number_of_nodes)
{
  Ogl::default_state();
  Ogl::shader_bind(&shader_dir_light);
  
  for(size_t n = 0; n < number_of_nodes; ++n)
  {
    // Render node.
    const Node *curr_node = &nodes[n];
    assert(curr_node);
    
    glUniformMatrix4fv(uni_dir_wvp,   1, GL_FALSE, curr_node->wvp);
    glUniformMatrix4fv(uni_dir_world, 1, GL_FALSE, curr_node->world_mat);
    Ogl::error_check("Setting directional light uniforms", &std::cout);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, curr_node->diffuse_id);
    
    Ogl::filtering_apply(filtering);
    Ogl::vertex_buffer_bind(curr_node->vbo, &vertex_format, &shader_dir_light);
    
    const GLsizei count = curr_node->vbo.number_of_entries / vertex_format.number_of_attributes;
    
    glDrawArrays(GL_TRIANGLES, 0, count);
  }
  
  Ogl::error_check("Drawing dir light nodes", &std::cout);  
}


} // ns