#ifndef MATERIAL_RENDERER_INCLUDED_40B58250_DD62_4792_9DE7_554E728832C5
#define MATERIAL_RENDERER_INCLUDED_40B58250_DD62_4792_9DE7_554E728832C5


#include <stdint.h>


namespace Rendering {


/*
  Takes an array of draw calls,
  and processes them into an arranged list.
*/
struct Ordered_draw_calls
{
  uint32_t    *material_ids;
  uint32_t    *size;
};


void
material_draw_call_arrage_by_materials();


/*!
  Takes an array of materials.
  and puts them into program_id order.
*/
void
material_order_optimize();


}


#endif // inc guard