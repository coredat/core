#ifndef CHUNK_INCLUDED_DC879174_5363_4EFE_8AA0_D8511499FCEF
#define CHUNK_INCLUDED_DC879174_5363_4EFE_8AA0_D8511499FCEF


#include <stddef.h>


namespace Core {
namespace Memory {


struct Chunk
{
  void    *start_of_chunk;
  size_t   bytes_in_chunk;
};


} // ns
} // ns


#endif // inc guard