#include "input_pool.hpp"
#include <cstring>
#include <assert.h>


namespace Context_data {


void
input_data_init(Input_pool *data)
{
  memset(data->controllers, 0, sizeof(data->controllers));
  memset(data->mice, 0, sizeof(data->mice));
  memset(data->keyboard, (uint32_t)Core::Button_state::down, sizeof(data->keyboard));
}


void
input_data_update_controller(Input_pool *pool,
                             const uint32_t controller,
                             const Game_controller *controller_data)
{
  memcpy(&pool->controllers[controller], controller_data, sizeof(Game_controller));
}


Game_controller*
input_data_get_controller(Input_pool *pool, const uint32_t controller)
{
  assert(controller < pool->controller_count);

  return &pool->controllers[controller];
}



} // ns