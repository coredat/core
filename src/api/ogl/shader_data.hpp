#ifdef OP_BUFFER_API_OGL
#ifndef SHADER_DATA_INCLUDED_86592DF9_15A2_4EF8_922E_C432BCA6B1F0
#define SHADER_DATA_INCLUDED_86592DF9_15A2_4EF8_922E_C432BCA6B1F0


#include "../../data/fwd.hpp"


namespace op {
namespace ogl {


void
shader_data_create(context_data *context, void *data);


void
shader_data_destroy(context_data *context, void *data);


void
shader_data_bind(context_data *context, void *data);


} // ns
} // ns


#endif // inc guard
#endif // api gaurd
