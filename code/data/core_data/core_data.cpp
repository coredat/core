#include "core_data.hpp"
#include <assert.h>


namespace
{
  Core_data::Core *core_data;
}


namespace Core_data {


Core*
get_core_data()
{
  return core_data;
}


void
set_core_data(Core_data::Core *core)
{
  core_data = core;
}


Input_data*
get_input_data(Core_data::Core *core_data)
{
  assert(core_data && core_data->input_data);
  return core_data->input_data;
}


} // ns