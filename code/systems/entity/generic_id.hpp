#ifndef GENERIC_ID_INCLUDED_41DA909C_56FC_408D_8034_7A963FC2520D
#define GENERIC_ID_INCLUDED_41DA909C_56FC_408D_8034_7A963FC2520D


#include <stdint.h>
#include <stddef.h>


namespace Generic_id {


/*!
  Sets the ids in sequential order.
  \param ids[] list of ids.
  \param number_of_ids how many ids in the array.
*/
void
ids_init_sequentially(uint32_t ids[], const uint32_t number_of_ids);


} // ns


#endif // inc guard