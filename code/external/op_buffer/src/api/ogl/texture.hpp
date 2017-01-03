#ifdef OP_BUFFER_API_OGL
#ifndef TEXTURE_INCLUDED_BA03AC6F_B4AA_468C_B416_E47318A109D1
#define TEXTURE_INCLUDED_BA03AC6F_B4AA_468C_B416_E47318A109D1


#include "../../data/fwd.hpp"


namespace op {
namespace ogl {


void
texture_create(context_data *context, void *data);


void
texture_update(context_data *context, void *data);


void
texture_destroy(context_data *context, void *data);


void
texture_bind(context_data *context, void *data);


} // ns
} // ns


#endif // inc guard
#endif // api guard
