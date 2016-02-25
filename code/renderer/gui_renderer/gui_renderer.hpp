#ifndef GUI_RENDERER_INCLUDED_621BBF5D_A351_4F38_8EC9_E060A4ACD47B
#define GUI_RENDERER_INCLUDED_621BBF5D_A351_4F38_8EC9_E060A4ACD47B


#include "gui_node.hpp"
#include <stdint.h>


namespace Gui_renderer {


void
initialize();


void
de_initialize();


void
render_gui_nodes(const Node nodes[],
                 const uint32_t number_of_nodes);


} // ns


#endif // inc guard