#ifndef TEXT_RENDERER_INCLUDED_51461528_5B88_43A0_925E_EE0150D4A5C3
#define TEXT_RENDERER_INCLUDED_51461528_5B88_43A0_925E_EE0150D4A5C3


#include <stdint.h>
#include <math/mat/mat_types.hpp>
#include "text_draw_call.hpp"


namespace Text_renderer {


void
initialize();


void
reset();


uint32_t
render(const math::mat4 &view_proj_mat,
       const Draw_call calls[],
       const uint32_t number_of_calls);


} // ns


#endif // inc guard