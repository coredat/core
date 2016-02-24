#include "generic_id.hpp"


namespace Generic_id {


void
ids_init_sequentially(uint32_t ids[], const uint32_t number_of_ids)
{
  for(uint32_t i = 0; i < number_of_ids; ++i)
  {
    ids[i] = i;
  }
}


} // ns