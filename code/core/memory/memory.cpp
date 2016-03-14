#include "memory.hpp"
#include <iostream>
#include <assert.h>


using namespace Core::Memory::Detail;


namespace
{
  uint8_t *memory_buffer(nullptr);
  size_t   size_of_buffer(0);
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
    
    Chunk_header *header   = reinterpret_cast<Chunk_header*>(memory_buffer);
    header->prev           = nullptr;
    header->next           = nullptr;
    header->size_of_chunk  = number_of_bytes_to_reserve - sizeof(Chunk_header);
    header->available      = true;
    header->start_of_chunk = &memory_buffer[0 + sizeof(Chunk_header)];
    
    return true;
  }
  
  return false;
}


Chunk
request_chunk(const size_t request_size)
{
  Chunk chunk;
  Chunk_header *this_header = reinterpret_cast<Chunk_header*>(memory_buffer);

  while(true)
  {
    if(this_header->available == true)
    {
      // Is size big enough.
      if(this_header->size_of_chunk >= (request_size + sizeof(Chunk_header)))
      {
        // Shrink this chunk down.
        const size_t size_of_chunk_to_split = this_header->size_of_chunk;
        
        this_header->size_of_chunk = request_size;
        this_header->available = false;
        
        uint8_t *next_chunk_start = &reinterpret_cast<uint8_t*>(this_header->start_of_chunk)[this_header->size_of_chunk];
        Chunk_header *next_chunk = reinterpret_cast<Chunk_header*>(next_chunk_start);
        
        next_chunk->prev = this_header;
        next_chunk->available = true;
        next_chunk->size_of_chunk = size_of_chunk_to_split - request_size - sizeof(Chunk_header);
        next_chunk->start_of_chunk = (void*)(next_chunk_start + sizeof(Chunk_header));
        
        this_header->next = next_chunk;
        
        chunk.start_of_chunk = this_header->start_of_chunk;
        chunk.bytes_in_chunk = request_size;
        
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


void
return_chunk(const Chunk chunk)
{
  // Find the chunk that has a matching start/size
  Chunk_header *header = reinterpret_cast<Chunk_header*>(memory_buffer);
  
  while(header)
  {
    if(header->size_of_chunk == chunk.bytes_in_chunk && header->start_of_chunk == chunk.start_of_chunk)
    {
      header->available = true;
      
      // We need to check if we should merge this chunk
      // with the previous or next chunk.
      Chunk_header *prev = header->prev;
      Chunk_header *next = header->next;
      
      // Merge chunk with previous.
      if(prev && prev->available)
      {
        prev->next = next;
        prev->size_of_chunk += sizeof(Chunk_header) + header->size_of_chunk;
        header = prev;
        prev = nullptr;
      }
      
      // Merge chunk with next
      if(next && next->available)
      {
        header->next = next->next;
        header->size_of_chunk += sizeof(Chunk_header) + next->size_of_chunk;
      }
    }
  
    header = header->next;
  }
}


uint32_t
number_of_chunks()
{
  assert(memory_buffer);
  
  uint32_t number_of_chunks(0);
  
  Chunk_header *header = reinterpret_cast<Chunk_header*>(memory_buffer);
  
  while(header)
  {
    header = header->next;
    ++number_of_chunks;
  }
  
  return number_of_chunks;
}


Chunk
get_chunk(const uint32_t index)
{
  Chunk return_chunk;
  
  uint32_t number_of_chunks(0);
  
  Chunk_header *header = reinterpret_cast<Chunk_header*>(memory_buffer);
  
  while(header)
  {
    if(number_of_chunks == index)
    {
      return_chunk.bytes_in_chunk = header->size_of_chunk;
      return_chunk.start_of_chunk = header->start_of_chunk;
    }  
  
    header = header->next;
    ++number_of_chunks;
  }
  
  return return_chunk;
}


size_t
get_available_space()
{
  size_t total_space(0);
  
  Chunk_header *header = reinterpret_cast<Chunk_header*>(memory_buffer);
  
  while(header)
  {
    if(header->available)
    {
      total_space += header->size_of_chunk;
    }
    
    header = header->next;
  }
  
  return total_space;
}


size_t
get_largest_chunk_size()
{
  size_t largest_chunk(0);
  
  Chunk_header *header = reinterpret_cast<Chunk_header*>(memory_buffer);
  
  while(header)
  {
    if(largest_chunk < header->size_of_chunk)
    {
      largest_chunk = header->size_of_chunk;
    }
  
    header = header->next;
  }
  
  return largest_chunk;
}


} // ns
} // ns