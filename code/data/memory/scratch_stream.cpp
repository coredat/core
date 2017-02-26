#include <data/memory/scratch_stream.hpp>
#include <utilities/alignment.hpp>
#include <utilities/logging.hpp>
#include <cstring>


namespace
{
  void *data = nullptr;
  void *curr_data = nullptr;
  size_t size_of_data = 0;
}


namespace Memory {
namespace Scratch_stream {


void
initialize(void *start_of_data, const size_t size_of_pool)
{
  data = start_of_data;
  curr_data = data;
  size_of_data = size_of_pool;
  
  LOG_TODO_ONCE("Whole bunch of things that need fixing.")
}


void
reset()
{
  curr_data = data;
}


void*
open()
{
  void *next_slot = nullptr;

  // 16 bytes align data
  next_slot = (void*)lib::align::get_boundry_16((uintptr_t)curr_data);
  
  return next_slot;
}


bool
add(const void *data, const size_t bytes_of_data)
{

  
  return true;
}


void
close()
{
  
}


} // ns
} // ns
