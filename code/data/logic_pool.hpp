#ifndef LOGIC_POOL_INCLUDED_2879D607_1430_4D5C_8806_974D5294E9F5
#define LOGIC_POOL_INCLUDED_2879D607_1430_4D5C_8806_974D5294E9F5


#include <cstddef>
#include <systems/logic/logic_base.hpp>


#define LOGIC_POOL_SIZE 512
#define LOGIC_POOL_STORAGE_SIZE 128


namespace Data {


struct Logic_pool
{
  uint8_t storage[LOGIC_POOL_SIZE * 128];
  void *free_list[LOGIC_POOL_SIZE];
  
  Logic::Base *start_ups[LOGIC_POOL_SIZE];
  Logic::Base *updates[LOGIC_POOL_SIZE];

  const std::size_t storage_size = LOGIC_POOL_STORAGE_SIZE;
  const std::size_t size = LOGIC_POOL_SIZE;
};


/*!
  Initialises the pool.
*/
void
logic_pool_init(Logic_pool *pool);


} // ns


#endif // inc guard