#include "input_pool.hpp"
#include <cstring>


namespace Core_data {


void
input_data_init(Input_pool *data)
{
  memset(data, 0, sizeof(data->controllers));
}


void
input_data_update_controller(Input_pool *pool,
                             const size_t controller,
                             const Controller *controller_data)
{
  memcpy(&pool[controller], controller_data, sizeof(Controller));
}



} // ns