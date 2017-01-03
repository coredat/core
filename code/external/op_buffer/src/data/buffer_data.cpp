#include "buffer_data.hpp"
#include "../common/command_structures.hpp"


namespace op {


bool
buffer_data::has_data() const
{
  return buffer_read_pointer < buffer_size;
}


bool
buffer_data::has_capacity(const size_t sizeof_data) const
{
  return (buffer_capacity - buffer_size) > sizeof_data;
}


void
buffer_data::reset()
{
  buffer_read_pointer = 0;
  buffer_size = 0;
}


void
buffer_data::initial_reserve()
{
  if(buffer_capacity == 0)
  {
    constexpr size_t inital_size = 64;
    buffer_data = (uint8_t*)alloc_cb(inital_size, user_data);

    if(buffer_data)
    {
      buffer_capacity = inital_size;
      memset(buffer_data, 0, sizeof(uint8_t) * buffer_size);
    }
  }
}


void
buffer_data::destroy_buffer()
{
  destroy_cb(buffer_data, user_data);
}


void
buffer_data::write_data(void *data, size_t sizeof_data)
{
  if(!has_capacity(sizeof_data))
  {
    const size_t new_capacity = buffer_capacity * 2;

    void *new_memory = realloc_cb(
      new_capacity,
      buffer_data,
      user_data
    );

    buffer_capacity = 0;

    if(new_memory)
    {
      buffer_data = (uint8_t*)new_memory;
      buffer_capacity = new_capacity;
    }
  }

  if(has_capacity(sizeof_data))
  {
    void *next = &buffer_data[buffer_size];

    memcpy(
      next,
      data,
      sizeof_data
    );
    
    buffer_size = (buffer_size + (sizeof_data + 15)) &~ 0x0F;
  }
  else
  {
    assert(false);
  }
}


void*
buffer_data::read_data()
{
  void *next = &buffer_data[buffer_read_pointer];

  command::cmd_unknown *uk_cmd = reinterpret_cast<command::cmd_unknown*>(next);

  buffer_read_pointer += uk_cmd->size + 15;
  buffer_read_pointer = buffer_read_pointer &~ 0x0F;

  return uk_cmd;
}


} // ns
