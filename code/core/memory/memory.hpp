#ifndef MEMORY_INCLUDED_B1124BD0_5912_4EE6_8A0C_CA04EA036145
#define MEMORY_INCLUDED_B1124BD0_5912_4EE6_8A0C_CA04EA036145


#include "chunk.hpp"
#include <stddef.h>
#include <stdint.h>


// TODO:
// ** Move this out of Core, should not be part of public interface! ** //


namespace Core {
namespace Memory {


namespace Detail {

// This is for testing only.
// Not for public use.
struct Chunk_header
{
  Chunk_header *next;
  Chunk_header *prev;
  void*         start_of_chunk;
  size_t        size_of_chunk;
  bool          available;
};

}


/*!
  Allocate a large chunk of memory for use.
  \param number_of_bytes_to_reserve How many bytes should you allocate.
  \return true if we allocated the requested size.
*/
bool [[deprecated("Use memory pool in data instead.")]]
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


/*!
  Return the number of chunks that have been allocated.
  This method is for debugging/integrity checks.
  \return number of chunks.
*/
uint32_t
number_of_chunks();


/*!
  Get the details of the chunk
  This method is for debugging/integrity checks.
  \return number of chunks.
*/
Chunk
get_chunk(const uint32_t index);


/*!
  Returns the total number of free bytes in the pool.
  of all the combined free chunks.
*/
size_t
get_available_space();


/*!
 Returns the single biggest chunk size.
*/
size_t
get_largest_chunk_size();


} // ns
} // ns


#endif // inc guard