#ifndef GENERIC_ID_INCLUDED_41DA909C_56FC_408D_8034_7A963FC2520D
#define GENERIC_ID_INCLUDED_41DA909C_56FC_408D_8034_7A963FC2520D


#include <cstddef>


namespace Generic_id {


/*!
  Sets the ids in sequential order.
  \param ids[] list of ids.
  \param number_of_ids how many ids in the array.
*/
void
ids_init_sequentially(std::size_t ids[], const std::size_t number_of_ids);


} // ns


#endif // inc guard