#ifndef CHUNK_INCLUDED_DC879174_5363_4EFE_8AA0_D8511499FCEF
#define CHUNK_INCLUDED_DC879174_5363_4EFE_8AA0_D8511499FCEF


#include <stddef.h>


namespace Core {
namespace Memory {


struct Chunk
{
  void    *start_of_chunk = nullptr;
  size_t   bytes_in_chunk = 0;
};


} // ns
} // ns


#endif // inc guard