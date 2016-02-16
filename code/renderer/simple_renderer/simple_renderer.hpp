#ifndef SIMPLE_RENDERER_INCLUDED_BE3F678A_E796_4175_8064_BFAAD83329BD
#define SIMPLE_RENDERER_INCLUDED_BE3F678A_E796_4175_8064_BFAAD83329BD


#include "simple_renderer_node.hpp"
#include <stddef.h>


/*!
  Simple Renderer
  --
  These renderer's are data driven and contain little state, they will
  simply render what is passed into the function, onto the already
  bound buffer.
*/
namespace Simple_renderer {


/*!
  Initalize the simple renderer.
  This needs to be called before you can start rendering things.
*/
void
initialize();


/*!
  Given a valid context this will render the given nodes to the screen.
  \param nodes[] The rendering nodes which to render.
  \param number_of_nodes How many nodes contained in the array.
*/
void
render_nodes_fullbright(const Node nodes[], const size_t number_of_nodes);


/*!
  Given a valid context this will render the given nodes to the screen.
  \param nodes[] The rendering nodes which to render.
  \param number_of_nodes How many nodes contained in the array.
*/
void
render_nodes_directional_light(const Node nodes[], const size_t number_of_nodes);


} // ns


#endif // inc guard