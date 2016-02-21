#include "input_pool.hpp"
#include <cstring>
#include <assert.h>


namespace Core_data {


void
input_data_init(Input_pool *data)
{
  memset(data, 0, sizeof(data->controllers));
}


void
input_data_update_controller(Input_pool *pool,
                             const size_t controller,
                             const Game_controller *controller_data)
{
  memcpy(&pool->controllers[controller], controller_data, sizeof(Game_controller));
}


Game_controller*
input_data_get_controller(Input_pool *pool, const size_t controller)
{
  assert(controller < pool->size);

  return &pool->controllers[controller];
}



} // ns