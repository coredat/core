#ifdef OP_BUFFER_API_OGL
#ifndef TEXTURE_FILTER_INCLUDED_FA745382_9532_4D0D_8E12_8E410E72B9AC
#define TEXTURE_FILTER_INCLUDED_FA745382_9532_4D0D_8E12_8E410E72B9AC


#include "../../data/fwd.hpp"


namespace op {
namespace ogl {


void
texture_filter_create(context_data *context, void *data);


void
texture_filter_update(context_data *context, void *data);


void
texture_filter_destroy(context_data *context, void *data);


void
texture_filter_bind(context_data *context, void *data);


} // ns
} // ns


#endif // inc guard
#endif // api guard
