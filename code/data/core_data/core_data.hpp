#ifndef CORE_DATA_INCLDUED_3B7A1761_B49B_41D6_B7D9_7439A356614E
#define CORE_DATA_INCLDUED_3B7A1761_B49B_41D6_B7D9_7439A356614E


#include "input_pool.hpp"


namespace Core_data {


struct Core
{
  Input_pool          *input_pool;
}; // struct


void
core_data_init(Core *data);


Core*
get_core_data();


void
set_core_data(Core_data::Core *core);


Input_pool*
get_input_data(Core_data::Core *core_data);


} // ns



#endif // inc gaurd