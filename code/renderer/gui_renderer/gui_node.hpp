#ifndef GUI_NODE_INCLUDED_9CFF639A_4B47_470C_B5DB_5980263ADD8E
#define GUI_NODE_INCLUDED_9CFF639A_4B47_470C_B5DB_5980263ADD8E


#include <graphics_api/ogl/ogl_texture.hpp>


namespace Gui_renderer {


struct Node
{
  float           quad_env[4];
  float           color[4];
  Ogl::Texture    diffuse;
};


} // ns


#endif // inc guard