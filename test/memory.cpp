#include "test_framework/catch.hpp"
#include <core/memory/memory.hpp>


TEST_CASE("Memory pool allocate and deallocates correctly.")
{
  constexpr size_t bytes_in_pool        = 1 << 17;
  constexpr size_t size_of_chunk_header = sizeof(Core::Memory::Detail::Chunk_header);
  constexpr size_t size_of_01           = 1024;
  constexpr size_t size_of_02           = 128;
  constexpr size_t size_of_03           = 512;
  
  uint32_t number_of_chunks(0);
  
  REQUIRE(Core::Memory::initialize(bytes_in_pool));
  
  REQUIRE(Core::Memory::get_largest_chunk_size() == (bytes_in_pool - size_of_chunk_header));

  // Allocate some memory.
  {
    ++number_of_chunks;
    Core::Memory::Chunk chunk_01 = Core::Memory::request_chunk(size_of_01);
    REQUIRE(chunk_01.bytes_in_chunk == size_of_01);
    
    ++number_of_chunks;
    Core::Memory::Chunk chunk_02 = Core::Memory::request_chunk(size_of_02);
    REQUIRE(chunk_02.bytes_in_chunk == size_of_02);
    
    ++number_of_chunks;
    Core::Memory::Chunk chunk_03 = Core::Memory::request_chunk(size_of_03);
    REQUIRE(chunk_03.bytes_in_chunk == size_of_03);
  }

  
  SECTION("Chunk integrity")
  {
    // If getting a chunk does not return the correct size
    // Then memseting that chunk could wipe out a header.
    // So clearing all the chunk memory should be a good indicator if
    // we've splatted any memory.
  
    REQUIRE(Core::Memory::number_of_chunks() == number_of_chunks + 1);

    Core::Memory::Chunk get_chunk_01 = Core::Memory::get_chunk(0);
    memset(get_chunk_01.start_of_chunk, 0, get_chunk_01.bytes_in_chunk);
    REQUIRE(get_chunk_01.bytes_in_chunk == size_of_01);
    
    Core::Memory::Chunk get_chunk_02 = Core::Memory::get_chunk(1);
    memset(get_chunk_02.start_of_chunk, 0, get_chunk_02.bytes_in_chunk);
    REQUIRE(get_chunk_02.bytes_in_chunk == size_of_02);
    
    Core::Memory::Chunk get_chunk_03 = Core::Memory::get_chunk(2);
    memset(get_chunk_03.start_of_chunk, 0, get_chunk_03.bytes_in_chunk);
    REQUIRE(get_chunk_03.bytes_in_chunk == size_of_03);
    
    REQUIRE(Core::Memory::number_of_chunks() == number_of_chunks + 1);
  }
  
  
  SECTION("Checking memory size")
  {
    const size_t allocated_size = size_of_01 + size_of_02 + size_of_03 + (size_of_chunk_header * (number_of_chunks + 1));
    
    REQUIRE(Core::Memory::get_largest_chunk_size() == (bytes_in_pool - allocated_size));
    REQUIRE(Core::Memory::get_available_space() == (bytes_in_pool - allocated_size));
    
  }

  
  SECTION("Return some memory")
  {
    Core::Memory::Chunk middle_chunk_to_return = Core::Memory::get_chunk(1);
    Core::Memory::return_chunk(middle_chunk_to_return);
    
    // Since we returned this memory to the available pool it should be seen in the total available memory.
    const size_t allocated_size = size_of_01 + size_of_03 + (size_of_chunk_header * (number_of_chunks + 1));
    REQUIRE(Core::Memory::get_available_space() == (bytes_in_pool - allocated_size));
    REQUIRE(Core::Memory::number_of_chunks() == number_of_chunks + 1);
    
    // Returning the last chunk should see the chunks merged internally.
    Core::Memory::Chunk end_chunk_to_return = Core::Memory::get_chunk(2);
    Core::Memory::return_chunk(end_chunk_to_return);
    
    // Since we returned this memory to the available pool it should be seen in the total available memory.
    const size_t final_allocated_size = size_of_01 + (size_of_chunk_header * (2));
    REQUIRE(Core::Memory::get_available_space() == (bytes_in_pool - final_allocated_size));
    REQUIRE(Core::Memory::number_of_chunks() == number_of_chunks - 1);
  }
}