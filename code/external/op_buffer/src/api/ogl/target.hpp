#ifdef OP_BUFFER_API_OGL
#ifndef TARGET_INCLUDED_6358044E_6C90_4E95_B7DD_F71B9BDEAC4B
#define TARGET_INCLUDED_6358044E_6C90_4E95_B7DD_F71B9BDEAC4B


#include "../../data/fwd.hpp"


namespace op {
namespace ogl {


void
target_create(context_data *context, void *data);


void
target_update(context_data *context, void *data);


void
target_destroy(context_data *context, void *data);


void
target_clear(context_data *context, void *data);


void
target_bind(context_data *context, void *data);


} // ns
} // ns


#endif // inc guard
#endif // ogl guard
