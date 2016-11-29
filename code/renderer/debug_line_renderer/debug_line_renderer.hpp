#ifndef DEBUG_LINE_RENDERER_INCLUDED_2475B016_9D5B_4FFD_AE7B_4BF5DA6A6C29
#define DEBUG_LINE_RENDERER_INCLUDED_2475B016_9D5B_4FFD_AE7B_4BF5DA6A6C29


#include "debug_line_renderer_node.hpp"
#include <graphics_api/gfx_fwd.hpp>
#include <stdint.h>


/*!
  Debug Line Renderer
  --
  This renderer draws colored lines from point a to point b.
  This is meant for debugging
*/
namespace Debug_line_renderer {


/*!
  Initalize the line renderer.
  This needs to be called before you can start rendering things.
*/
void
initialize(Graphics_api::Context *ctx);


/*!
  Given a valid context this will build the required data for the render step.
  \param nodes[] Array of line nodes that you wish to render
  \param number_of_lines How many nodes contained in the array.
*/
void
add_lines(const Line_node nodes[], const uint32_t number_of_lines);


/*!
  Given a valid context this will render the given nodes to the screen.
*/
void
render(const float wvp_matrix[16]);


} // ns


#endif // inc guard