#ifdef OP_BUFFER_API_OGL
#ifndef DEBUG_INCLUDED_91871EF8_E96C_472A_B610_D7FA633B38C9
#define DEBUG_INCLUDED_91871EF8_E96C_472A_B610_D7FA633B38C9


#include "../../data/fwd.hpp"


namespace op {
namespace ogl {


void
debug_marker_push(context_data *context, void *data);


void
debug_marker_pop(context_data *context, void *data);


} // ns
} // ns


#endif // inc guard
#endif // api guard
