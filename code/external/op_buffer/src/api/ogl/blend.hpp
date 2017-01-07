#ifdef OP_BUFFER_API_OGL
#ifndef BLEND_INCLUDED_EE57A4A5_5805_4BEB_AEFA_C7FC08A2B812
#define BLEND_INCLUDED_EE57A4A5_5805_4BEB_AEFA_C7FC08A2B812


#include "../../data/fwd.hpp"


namespace op {
namespace ogl {


void
blend_create(context_data *context, void *data);


void
blend_update(context_data *context, void *data);


void
blend_destroy(context_data *context, void *data);


void
blend_bind(context_data *context, void *data);


} // ns
} // ns


#endif // inc guard
#endif // api guard
