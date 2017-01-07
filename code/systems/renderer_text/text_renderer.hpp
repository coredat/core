#ifndef TEXT_RENDERER_INCLUDED_51461528_5B88_43A0_925E_EE0150D4A5C3
#define TEXT_RENDERER_INCLUDED_51461528_5B88_43A0_925E_EE0150D4A5C3


#include <op/fwd.hpp>
#include <stdint.h>
#include <math/mat/mat_types.hpp>
#include "text_draw_call.hpp"


namespace Text_renderer {


void
initialize(opContext *ctx,
           opBuffer *buf);


uint32_t
render(const math::mat4 &view_proj_mat,
       const uint32_t cam_cull_mask,
       /*const*/ Draw_call calls[], // non cost while op buffer has no memory support.
       const uint32_t number_of_calls,
       opContext *ctx,
       opBuffer *buf);


} // ns


#endif // inc guard