#ifdef OP_BUFFER_API_OGL
#ifndef GEOMETRY_INCLUDED_6C7B5FB1_5009_4C3C_84B7_25F7F38E0EB7
#define GEOMETRY_INCLUDED_6C7B5FB1_5009_4C3C_84B7_25F7F38E0EB7


#include "../../data/fwd.hpp"


namespace op {
namespace ogl {


void
geometry_create(context_data *context, void *data);


void
geometry_update(context_data *context, void *data);


void
geometry_destroy(context_data *context, void *data);


void
geometry_bind(context_data *context, void *data);


} // ns
} // ns


#endif // inc guard
#endif // api guard
