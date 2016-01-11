#include "generic_id.hpp"


namespace Generic_id {


void
ids_init_sequentially(std::size_t ids[], const std::size_t number_of_ids)
{
  for(std::size_t i = 0; i < number_of_ids; ++i)
  {
    ids[i] = i;
  }
}


} // ns