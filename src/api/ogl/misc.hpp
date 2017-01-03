#ifdef OP_BUFFER_API_OGL
#ifndef MISC_INCLUDED_5F62A7E8_A8B3_4AF8_9BBD_159777D328C6
#define MISC_INCLUDED_5F62A7E8_A8B3_4AF8_9BBD_159777D328C6s


#include "shader.hpp"
#include "details/ogl_common.hpp"
#include "details/error_code_to_text.hpp"
#include "details/op_common.hpp"
#include "details/op_to_ogl.hpp"
#include "descs/descs.hpp"


namespace op {
namespace ogl {


void
viewport_set(context_data *context, void *data);


}
}


#endif // inc guard
#endif // api guard
