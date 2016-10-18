#include <data/memory/memory_data.hpp>
#include <utilities/memory.hpp>
#include <assert.h>


namespace
{
  util::memory_pool   memory_pool;
  util::memory_chunk  scratch_chunk;
  
  void                *scratch_pointer       = nullptr;
  void                *scratch_pointer_start = nullptr;
  
  size_t              scratch_size = 0;
  size_t              scratch_capacity = 0;
  
  size_t              max_use_stat = 0;
}


namespace Memory {


void
memory_initialize(const size_t pool_bytes, const size_t scratch_size)
{
  if(!memory_pool.header)
  {
    memory_pool           = util::memory_pool_create(pool_bytes + scratch_size);
    scratch_chunk         = util::memory_pool_get_chunk(&memory_pool, scratch_size, "scratch");
    scratch_pointer       = scratch_chunk.chunk_start;
    scratch_pointer_start = scratch_pointer;
    scratch_capacity      = scratch_chunk.bytes_in_chunk;
  }
}


void
scratch_reset()
{
  const size_t used = _get_scratch_bytes_used();
  if(used > max_use_stat) { max_use_stat = used; }

  scratch_pointer = scratch_pointer_start;
  scratch_size    = 0;
}


void*
scratch_alloc(const size_t bytes)
{
  if(bytes == 0)
  {
    return nullptr;
  }

  if((scratch_size + bytes) < scratch_capacity)
  {
    scratch_size += bytes;
    
    void *ptr = scratch_pointer;
    
    scratch_pointer = util::mem_offset(scratch_pointer, bytes);
    
    return ptr;
  }
  
  assert(false);
  return nullptr;
}


void*
scratch_alloc_aligned(const size_t bytes)
{
  void *ptr = scratch_alloc(bytes + 16);
  
  return util::mem_next_16byte_boundry(ptr);
}


util::memory_chunk
request_memory_chunk(const size_t bytes, const char *name)
{
  assert(bytes);

  if(memory_pool.header)
  {
    return util::memory_pool_get_chunk(&memory_pool, bytes, name);
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


util::memory_pool*
_get_pool()
{
  return &memory_pool;
}


size_t
_get_scratch_bytes_used()
{
  return (uintptr_t)scratch_pointer - (uintptr_t)scratch_pointer_start;
}


size_t
_get_scratch_bytes_total()
{
  return scratch_capacity;
}


size_t
_get_scratch_max_used()
{
  return max_use_stat;
}


} // ns