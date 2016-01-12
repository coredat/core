#ifndef LOGIC_POOL_INCLUDED_2879D607_1430_4D5C_8806_974D5294E9F5
#define LOGIC_POOL_INCLUDED_2879D607_1430_4D5C_8806_974D5294E9F5


#include <systems/logic/logic_base.hpp>
#include <vector>
#include <cstddef>


#define LOGIC_POOL_SIZE 512
#define LOGIC_POOL_STORAGE_SIZE 128


namespace Data {


struct Logic_pool
{
  uint8_t storage[LOGIC_POOL_SIZE * 128];

  std::vector<void*> objects_in_use; // Using vectors while prototyping.
  std::vector<void*> free_list;
  
  const std::size_t storage_size = LOGIC_POOL_STORAGE_SIZE;
  const std::size_t size = LOGIC_POOL_SIZE;
};


/*!
  Initialises the pool.
*/
void
logic_pool_init(Logic_pool *pool);


/*!
  Returns a pointer to a valid location for a new object
*/
void*
logic_pool_get_slot(Logic_pool *pool);


} // ns


#endif // inc guard