#include "simple_renderer.hpp"
#include <string>
#include <utils/directory.hpp>
#include <assert.h>
#include "../graphics_api/texture_filtering.hpp"
#include "../graphics_api/ogl/ogl_texture_filtering.hpp"
#include "../graphics_api/ogl/ogl_shader.hpp"
#include "../graphics_api/ogl/ogl_shader_uniform.hpp"
#include "../graphics_api/ogl/ogl_common.hpp"
#include "../graphics_api/ogl/ogl_texture.hpp"
#include <iostream>


namespace
{
  renderer::shader dir_light;
  renderer::shader fullbright;
  renderer::vertex_format vertex_fmt;
  GLint uniTrans;
  
  Ogl::Shader shader_fullbright;
  Ogl::Shader shader_dir_light;
  
  GLint uni_wvp(0);
  GLint uni_world(0);
  
  Graphics_api::Texture_filtering filtering;
}


namespace Simple_renderer {


void
initialize()
{
  const std::string asset_path = util::get_resource_path() + "assets/";
  const auto full_code = renderer::shader_utils::get_shader_code_from_tagged_file(asset_path + "shaders/basic_fullbright.ogl");
  fullbright.load_shader(full_code);
  assert(fullbright.is_valid());

  uniTrans = glGetUniformLocation(fullbright.get_program_gl_id(), "wvp");

  const auto dir_code = renderer::shader_utils::get_shader_code_from_tagged_file(asset_path + "shaders/basic_dir_light.ogl");
  dir_light.load_shader(dir_code);
  assert(dir_light.is_valid());

  Ogl::shader_create(&shader_fullbright, full_code.vs_code.c_str(), full_code.cs_code.c_str(), full_code.ps_code.c_str(), &std::cout);
  Ogl::shader_create(&shader_dir_light, dir_code.vs_code.c_str(), dir_code.cs_code.c_str(), dir_code.ps_code.c_str(), &std::cout);
  Ogl::error_check("Building simple shaders.", &std::cout);

  Ogl::Shader_uniforms uniforms;
  Ogl::shader_uniforms_retrive(&uniforms, &shader_dir_light);
  Ogl::error_check("Getting uniforms from simple shaders", &std::cout);
  
  Ogl::shader_uniforms_get_uniform_index(&uni_wvp, &uniforms, "uni_wvp_mat");
  Ogl::shader_uniforms_get_uniform_index(&uni_world, &uniforms, "uni_world");
  Ogl::error_check("Finding uniforms from simple shaders", &std::cout);
  

  
  vertex_fmt.load_format({
    renderer::attr_format_desc{"in_vs_position",      renderer::attr_type::FLOAT3},
    renderer::attr_format_desc{"in_vs_texture_coord", renderer::attr_type::FLOAT2},
    renderer::attr_format_desc{"in_vs_normal",        renderer::attr_type::FLOAT3},
  });

  assert(vertex_fmt.is_valid());
  
  filtering.wrap_mode_s = Graphics_api::Wrap_mode::clamp;
  filtering.wrap_mode_t = Graphics_api::Wrap_mode::clamp;
  filtering.filtering   = Graphics_api::Filtering_mode::anisotropic;
}


void
render_nodes_fullbright(const Node nodes[],
                        const size_t number_of_nodes)
{
  renderer::reset();
  
  for(size_t n = 0; n < number_of_nodes; ++n)
  {
    // Render node.
    const Node *curr_node = &nodes[n];
    assert(curr_node);
    
    curr_node->vbo.bind(vertex_fmt, fullbright);                                // *hurt* need to know if this is a duplicate bind?
    fullbright.bind();
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, curr_node->diffuse_id);
    
    Ogl::filtering_apply(filtering);

    glUniform1i(glGetUniformLocation(fullbright.get_program_gl_id(), "diffuse_map"), 0);

    glUniformMatrix4fv(uniTrans, 1, GL_FALSE, curr_node->wvp);
    
    const GLsizei count = curr_node->vbo.get_number_entries() / vertex_fmt.get_number_of_entires(); // *hurt* Can be pre processed.
    
    /* IBO? */

    glDrawArrays(GL_TRIANGLES, 0, count);
  }
}


void
render_nodes_directional_light(const Node nodes[], const size_t number_of_nodes)
{
  renderer::reset();

  // TODO:
  // Lot of state changes that can be saved here. Setting the shader every node?
  dir_light.bind();
  
  const float color[3] {0.8f,0.7f,0.7f};
  dir_light.set_raw_data("dir_light.color", &color[0], sizeof(float) * 3);
  
  const float dir[3] {-0.707f,-0.707f,-0.707f};
  dir_light.set_raw_data("dir_light.direction", &dir[0], sizeof(float) * 3);

  const float amb = 0.75f;
  dir_light.set_raw_data("dir_light.ambient", &amb, sizeof(float) * 1);
  
  const float diff = 0.8f;
  dir_light.set_raw_data("dir_light.diffuse", &diff, sizeof(float) * 1);
  
  for(size_t n = 0; n < number_of_nodes; ++n)
  {
    // Render node.
    const Node *curr_node = &nodes[n];
    assert(curr_node);
    
    dir_light.set_raw_data("wvp", curr_node->wvp, sizeof(float) * 16);
    dir_light.set_raw_data("world", curr_node->world_mat, sizeof(float) * 16);
    
//    glUniformMatrix4fv(uni_wvp, 1, GL_FALSE, curr_node->wvp);
//    Ogl::error_check("set wvp.", &std::cout);
//  
//    glUniformMatrix4fv(uni_world, 1, GL_FALSE, curr_node->world_mat);
//    Ogl::error_check("set wvp.", &std::cout);

    // We need to rebind this because the other stuff is TODO
    dir_light.bind();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, curr_node->diffuse_id);
    
    Ogl::filtering_apply(filtering);
    
    curr_node->vbo.bind(vertex_fmt, dir_light); // *hurt* need to know if this is a duplicate bind?
    
    const GLsizei count = curr_node->vbo.get_number_entries() / vertex_fmt.get_number_of_entires(); // *hurt* Can be pre processed.
    
    /* IBO? */

    glDrawArrays(GL_TRIANGLES, 0, count);
  }  
}


} // ns