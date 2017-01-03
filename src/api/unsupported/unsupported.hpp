#ifndef UNSUPPORTED_API_INCLUDED_F1C7BE4D_6B50_427B_9FCE_9AF2FB725078
#define UNSUPPORTED_API_INCLUDED_F1C7BE4D_6B50_427B_9FCE_9AF2FB725078


#include "../../common/common.hpp"
#include "../../common/commands.hpp"
#include "../../data/context_data.hpp"
#include <stdio.h>


namespace op {
namespace unsupported {


static void
unsupported(context_data *context, void *data)
{
  context->log("unsupported feature");
  assert(false);
}


static void
initialize(
  op::command::cmd_api_function *api_array,
  size_t api_array_count
)
{
  for(size_t i = 0; i < api_array_count; ++i)
  {
    api_array[i] = unsupported;
  }
}


} // ns
} // ns


#endif // inc guard
