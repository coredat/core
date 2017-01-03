#ifdef OP_BUFFER_API_OGL
#ifndef SHADER_INCLUDED_B5B82674_4ACD_47D4_AABD_D8EF70998E2A
#define SHADER_INCLUDED_B5B82674_4ACD_47D4_AABD_D8EF70998E2A


#include "../../data/fwd.hpp"


namespace op {
namespace ogl {


void
shader_create(context_data *context, void *data);


void
shader_destroy(context_data *context, void *data);


void
shader_bind(context_data *context, void *data);


} // ns
} // ns


#endif // inc guard
#endif // api guard
