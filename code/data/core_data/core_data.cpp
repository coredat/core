#include "core_data.hpp"
#include <assert.h>


namespace Core_data {


Input_data*
get_input_data(Core_data::Core *core_data)
{
  assert(core_data && core_data->input_data);
  return core_data->input_data;
}


} // ns