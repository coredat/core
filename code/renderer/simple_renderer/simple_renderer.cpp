#include "simple_renderer.hpp"
#include <string>
#include <utils/directory.hpp>
#include <assert.h>


namespace
{
  renderer::shader dir_light;
  renderer::shader fullbright;
  renderer::vertex_format vertex_fmt;
  GLint uniTrans;
}


namespace Simple_renderer {


void
initialize()
{
  const std::string asset_path = util::get_resource_path() + "assets/";
  const auto code = renderer::shader_utils::get_shader_code_from_tagged_file(asset_path + "shaders/basic_fullbright.ogl");
  fullbright.load_shader(code);
  
  assert(fullbright.is_valid());

  vertex_fmt.load_format({
    renderer::attr_format_desc{"in_vs_position",      renderer::attr_type::FLOAT3},
    renderer::attr_format_desc{"in_vs_texture_coord", renderer::attr_type::FLOAT2},
    renderer::attr_format_desc{"in_vs_normal",        renderer::attr_type::FLOAT3},
  });
  assert(vertex_fmt.is_valid());
  
  uniTrans = glGetUniformLocation(fullbright.get_program_gl_id(), "wvp");
}


void
render_nodes_fullbright(const Node nodes[],
                        const std::size_t number_of_nodes)
{
  renderer::reset();
  
  for(std::size_t n = 0; n < number_of_nodes; ++n)
  {
    // Render node.
    const Node *curr_node = &nodes[n];
    assert(curr_node);
    
    fullbright.set_texture("diffuse_map", curr_node->diffuse_id);             // *hurt* need to know if this is a duplicate bind?
    curr_node->vbo.bind(vertex_fmt, fullbright);                              // *hurt* need to know if this is a duplicate bind?

    fullbright.bind();
  
    glUniformMatrix4fv(uniTrans, 1, GL_FALSE, curr_node->wvp);
    
    const GLsizei count = curr_node->vbo.get_number_entries() / vertex_fmt.get_number_of_entires(); // *hurt* Can be pre processed.
    
    /* IBO? */

    glDrawArrays(GL_TRIANGLES, 0, count);
  }
}


} // ns