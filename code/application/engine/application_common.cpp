#include "application_common.hpp"
#include <data/core_data/core_data.hpp>
#include <data/core_data/input_pool.hpp>
#include <systems/environment/environment.hpp>
#include <assert.h>


namespace Application {


void
common_think(
  Environment::Window *window,
  Core_data::Core *core_data)
{
  Environment::window_think(window);

  // Environment events.
  Environment::think(window, core_data->input_pool);
}


} // ns