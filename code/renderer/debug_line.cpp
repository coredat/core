#include "debug_line.hpp"
#include "debug_line_renderer/debug_line_renderer.hpp"



namespace Renderer {


void
debug_line(const float from[3], const float to[3], const float rgb_color[3])
{
  Debug_line_renderer::Line_node node;

  memcpy(node.position_from,  from,       sizeof(float) * 3);
  memcpy(node.position_to,    to,         sizeof(float) * 3);
  memcpy(node.color,          rgb_color,  sizeof(float) * 3);
  
  Debug_line_renderer::add_lines(&node, 1);
}


void
debug_line(const math::vec3 from_vec, const math::vec3 to_vec, const math::vec3 rgb_color_vec)
{
  float from[3];
  {
    from[0] = math::vec3_get_x(from_vec);
    from[1] = math::vec3_get_y(from_vec);
    from[2] = math::vec3_get_z(from_vec);
  }
  
  float to[3];
  {
    to[0] = math::vec3_get_x(to_vec);
    to[1] = math::vec3_get_y(to_vec);
    to[2] = math::vec3_get_z(to_vec);
  }
  
  float color[3];
  {
    color[0] = math::vec3_get_x(rgb_color_vec);
    color[1] = math::vec3_get_y(rgb_color_vec);
    color[2] = math::vec3_get_z(rgb_color_vec);
  }
  
  debug_line(from, to, color);
}


void
debug_line(const btVector3 &from_vec, const btVector3 &to_vec, const btVector3 &rgb_color_vec)
{
  float from[3];
  {
    from[0] = from_vec.x();
    from[1] = from_vec.y();
    from[2] = from_vec.z();
  }
  
  float to[3];
  {
    to[0] = to_vec.x();
    to[1] = to_vec.y();
    to[2] = to_vec.z();
  }
  
  float color[3];
  {
    color[0] = rgb_color_vec.x();
    color[1] = rgb_color_vec.y();
    color[2] = rgb_color_vec.z();
  }
  
  debug_line(from, to, color);
}


} // ns