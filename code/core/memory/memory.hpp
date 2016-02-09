#ifndef MEMORY_INCLUDED_B1124BD0_5912_4EE6_8A0C_CA04EA036145
#define MEMORY_INCLUDED_B1124BD0_5912_4EE6_8A0C_CA04EA036145


#include "chunk.hpp"
#include <stddef.h>


namespace Core {
namespace Memory {


/*!
  Allocate a large chunk of memory for use.
  \param number_of_bytes_to_reserve How many bytes should you allocate.
  \return true if we allocated the requested size.
*/
bool
initialize(const size_t number_of_bytes_to_reserve);


/*!
  Request a chunk of memory.
  \param request_size how big a chunk of memory you want.
  \return returns a chunk of memory. If it failed the chunks address will be null.
*/
Chunk
request_chunk(const size_t request_size);


/*!
  Return a chunk of memory to the pool.
  \param chunk the chunk of memory you wish to return.
*/
void
return_chunk(const Chunk chunk);


} // ns
} // ns


#endif // inc guard