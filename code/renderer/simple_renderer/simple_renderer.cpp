#include "simple_renderer.hpp"
#include <string>
#include <utils/directory.hpp>
#include <assert.h>
#include "../graphics_api/texture_filtering.hpp"
#include "../graphics_api/ogl/ogl_texture_filtering.hpp"


namespace
{
  renderer::shader dir_light;
  renderer::shader fullbright;
  renderer::vertex_format vertex_fmt;
  GLint uniTrans;
  
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

  const auto dir_code = renderer::shader_utils::get_shader_code_from_tagged_file(asset_path + "shaders/dir_light.ogl");
  dir_light.load_shader(dir_code);
  assert(dir_light.is_valid());

//  dir_light_wvp_mat = glGetUniformLocation(dir_light.get_program_gl_id(), "wvp");
//  dir_light_world_mat = glGetUniformLocation(dir_light.get_program_gl_id(), "world");
  
  vertex_fmt.load_format({
    renderer::attr_format_desc{"in_vs_position",      renderer::attr_type::FLOAT3},
    renderer::attr_format_desc{"in_vs_texture_coord", renderer::attr_type::FLOAT2},
    renderer::attr_format_desc{"in_vs_normal",        renderer::attr_type::FLOAT3},
  });

  assert(vertex_fmt.is_valid());
}


void
render_nodes_fullbright(const Node nodes[],
                        const std::size_t number_of_nodes)
{
  renderer::reset();
  
  filtering.wrap_mode_s = Graphics_api::Wrap_mode::clamp;
  filtering.wrap_mode_t = Graphics_api::Wrap_mode::clamp;
  filtering.filtering   = Graphics_api::Filtering_mode::anisotropic;
  
  for(std::size_t n = 0; n < number_of_nodes; ++n)
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
render_nodes_directional_light(const Node nodes[], const std::size_t number_of_nodes, const float* eye_pos_vec3)
{
  renderer::reset();
  
//  for(std::size_t n = 0; n < number_of_nodes; ++n)
//  {
//    // Render node.
//    const Node *curr_node = &nodes[n];
//    assert(curr_node);
//    
//    dir_light.set_raw_data("wvp", curr_node->wvp, sizeof(float) * 16);
//    dir_light.set_raw_data("world", curr_node->world_mat, sizeof(float) * 16);
//    dir_light.set_raw_data("eye_position", eye_pos_vec3, sizeof(float) * 3);
//    dir_light.set_texture("diffuse_map", curr_node->diffuse_id);  // *hurt* need to know if this is a duplicate bind?
//    
//    const float color[3] {0.8,0.7,0.7};
//    dir_light.set_raw_data("dir_light.color", &color[0], sizeof(float) * 3);
//    
//    const float dir[3] {0.707,0.707,0.0};
//    dir_light.set_raw_data("dir_light.direction", &dir[0], sizeof(float) * 3);
//
//    const float amb = 1.5f;
//    dir_light.set_raw_data("dir_light.ambient", &amb, sizeof(float) * 1);
//    
//    const float diff = 0.0f;
//    dir_light.set_raw_data("dir_light.direction", &diff, sizeof(float) * 1);
//    
//    curr_node->vbo.bind(vertex_fmt, dir_light);                  // *hurt* need to know if this is a duplicate bind?
//
//    dir_light.bind();
//    
//    const GLsizei count = curr_node->vbo.get_number_entries() / vertex_fmt.get_number_of_entires(); // *hurt* Can be pre processed.
//    
//    /* IBO? */
//
//    glDrawArrays(GL_TRIANGLES, 0, count);
//  }  
}


} // ns