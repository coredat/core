#include <data/global_data/memory_data.hpp>
#include <assert.h>


namespace
{
  util::memory_pool   memory_pool;
  util::memory_chunk  scratch_chunk;
  
  void                *scratch_pointer       = nullptr;
  void                *scratch_pointer_start = nullptr;
  
  size_t              scratch_size = 0;
  size_t              scratch_capacity = 0;
}


namespace memory {


void
memory_initialize(const size_t pool_bytes, const size_t scratch_size)
{
  if(!memory_pool.header)
  {
    memory_pool      = util::memory_pool_create(pool_bytes + scratch_size);
    scratch_chunk    = util::memory_pool_get_chunk(&memory_pool, scratch_size);
    scratch_pointer  = scratch_chunk.chunk_start;
    scratch_pointer_start = scratch_pointer;
    scratch_capacity = scratch_chunk.bytes_in_chunk;
  }
}


void
scratch_reset()
{
  scratch_pointer = scratch_pointer_start;
  scratch_size    = 0;
}


void*
scratch_alloc(const size_t bytes)
{
  if((scratch_size + bytes) < scratch_capacity)
  {
    scratch_size += bytes;
    
    void *ptr = scratch_pointer;
    
    scratch_pointer = (void*)((uintptr_t)scratch_pointer + bytes);
    
    return ptr;
  }
  
  assert(false);
  return nullptr;
}


void*
scratch_alloc_aligned(const size_t bytes)
{
  return nullptr;
}


util::memory_chunk
request_memory_chunk(const size_t bytes)
{
  if(memory_pool.header)
  {
    return util::memory_pool_get_chunk(&memory_pool, bytes);
  }
  
  return util::memory_chunk();
}


void
return_memory_chunk(util::memory_chunk *chunk)
{
  assert(chunk);

  if(chunk && memory_pool.header)
  {
    util::memory_pool_return_chunk(&memory_pool, chunk);
  }
}


} // ns