#include "generic_id.hpp"


namespace Generic_id {


void
ids_init_sequentially(size_t ids[], const size_t number_of_ids)
{
  for(size_t i = 0; i < number_of_ids; ++i)
  {
    ids[i] = i;
  }
}


} // ns