#ifdef OP_BUFFER_API_OGL
#ifndef DEVICE_INCLUDED_3675896B_2882_4D8B_A756_A554EE3A2AF8
#define DEVICE_INCLUDED_3675896B_2882_4D8B_A756_A554EE3A2AF8


#include "../../data/fwd.hpp"


namespace op {
namespace ogl {


void
device_init(context_data *context, void *data);


void
device_reset(context_data *context, void *data);


void
device_destroy(context_data *context, void *data);


} // ns
} // ns


#endif // inc guard
#endif // ogl guard
