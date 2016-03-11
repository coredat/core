#include "memory.hpp"
#include <iostream>
#include <assert.h>


namespace
{
  uint8_t *memory_buffer(nullptr);
  size_t   size_of_buffer(0);
  
  struct Chunk_header
  {
    Chunk_header *next;
    Chunk_header *prev;
    size_t        size_of_chunk;
    bool          available;
  };
}


namespace Core {
namespace Memory {


bool
initialize(const size_t number_of_bytes_to_reserve)
{
  memory_buffer = new uint8_t[number_of_bytes_to_reserve];
  
  if(memory_buffer)
  {
    size_of_buffer = number_of_bytes_to_reserve;
    
    Chunk_header *header  = reinterpret_cast<Chunk_header*>(&memory_buffer[0]);
    header->next          = nullptr;
    header->next          = nullptr;
    header->size_of_chunk = number_of_bytes_to_reserve - sizeof(Chunk_header);
    header->available     = true;
    
    return true;
  }
  
  return false;
}


Chunk
request_chunk(const size_t request_size)
{
  Chunk chunk;

  while(true)
  {
    Chunk_header *this_header = reinterpret_cast<Chunk_header*>(&memory_buffer[0]);
  
    if(this_header->available == true)
    {
      // Is size big enough.
      if(this_header->size_of_chunk >= request_size + sizeof(Chunk_header))
      {
        // Hooray we have found memory. We need to split this up now.
        uint8_t *start_of_avail_chunk = reinterpret_cast<uint8_t*>(this_header);
        uint8_t *next_chunk = &start_of_avail_chunk[request_size + sizeof(Chunk_header)];
        
        Chunk_header *next_chunk_header = reinterpret_cast<Chunk_header*>(next_chunk);
        Chunk_header *old_next_header = this_header->next;
        
        this_header->next = next_chunk_header;
        this_header->available = false;
        
        if(old_next_header)
        {
          old_next_header->prev = next_chunk_header;
        }
        
        chunk.bytes_in_chunk = request_size;
        chunk.start_of_chunk = start_of_avail_chunk;
        
        return chunk;
      }
    }

    // Next chunk if it exists.
    if(this_header->next)
    {
      this_header = this_header->next;
    }
    // No more chunks to search, no memory avail :(
    else
    {
      assert(false);
      break;
    }
  }
  
  return chunk;
}


} // ns
} // ns