#ifdef OP_BUFFER_API_OGL
#ifndef RASTERIZER_INCLUDED_7FF09720_A7C2_48A7_8CC3_D7DAAF83C01A
#define RASTERIZER_INCLUDED_7FF09720_A7C2_48A7_8CC3_D7DAAF83C01A


#include "../../data/fwd.hpp"


namespace op {
namespace ogl {


void
rasterizer_create(context_data *context, void *data);


void
rasterizer_update(context_data *context, void *data);


void
rasterizer_destroy(context_data *context, void *data);


void
rasterizer_bind(context_data *context, void *data);


} // ns
} // ns


#endif // inc guard
#endif // api guard
