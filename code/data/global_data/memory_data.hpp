#ifndef MEMORY_DATA_INCLUDED_997C23A2_93F6_48F4_B429_7670A915A5D7
#define MEMORY_DATA_INCLUDED_997C23A2_93F6_48F4_B429_7670A915A5D7


#include <utilities/memory_pool.hpp>
#include <stddef.h>


#define SCRATCH_ALIGNED_ALLOC(type, size) reinterpret_cast<type*>(::Memory::scratch_alloc_aligned(sizeof(type) * size));

#define SCRATCH_ALLOC(type, size) reinterpret_cast<type*>(::Memory::scratch_alloc_aligned(sizeof(type) * size));


namespace Memory {


/*!
  Initialize the memory pool
*/
void
memory_initialize(const size_t pool_byte,
                  const size_t scratch_size);


/*!
  Reset the scratch pool.
*/
void
scratch_reset();


/*!
  Allocate some memory.
*/
void*
scratch_alloc(const size_t bytes);


/*!
  Allocate some 16 byte alligned memory.
*/
void*
scratch_alloc_aligned(const size_t bytes);


/*!
  Get a chunk of memory for a pool to make to use.
*/
util::memory_chunk
request_memory_chunk(const size_t bytes, const char *name);


/*!
  Return a chunk to the pool.
*/
void
return_memory_chunk(util::memory_chunk *chunk);


/*!
  For debug only. Use request_memory_chunk if you want memory.
*/
util::memory_pool*
_get_pool();


/*!
  For debug only.
*/
size_t
_get_scratch_bytes_used();


/*!
  For debug only.
*/
size_t
_get_scratch_bytes_total();


/*!
  Max useage of the scratch pool.
*/
size_t
_get_scratch_max_used();


} // ns


#endif // inc guard