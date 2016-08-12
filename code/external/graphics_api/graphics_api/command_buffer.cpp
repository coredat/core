#include "command_buffer.hpp"
#include "api_detail/command_state.hpp"
#include "shader.hpp"
#include "texture.hpp"
#include "mesh.hpp"
#include <assert.h>
#include <cstring>


namespace Graphics_api {


void
command_buffer_create(Command_buffer *buffer, const size_t capacity_hint)
{
  buffer->buffer_data = new uint8_t[capacity_hint];
  
  buffer->bytes_used = 0;
  
  size_t *cap = const_cast<size_t*>(&buffer->bytes_capacity);
  *cap = capacity_hint;
}


void
command_buffer_destroy(Command_buffer *buffer)
{
  delete[] buffer->buffer_data;
}


namespace {


namespace Gfx = ::Graphics_api;


inline bool
insert_cmd(Gfx::Command_buffer *buf,
           const Gfx::Detail::Buffer_state state,
           const void *data,
           const size_t bytes_in_data)
{
  assert(buf);
  
  const size_t bytes_needed = sizeof(Gfx::Detail::Buffer_state) + bytes_in_data;
  const size_t total_bytes = bytes_needed + buf->bytes_used;
  
  // Can it fit into the data.
  if(total_bytes > buf->bytes_capacity)
  {
    return false;
  }
  
  // Add id and the data to the buffer.
  memcpy(&buf->buffer_data[buf->bytes_used], &state, sizeof(state));
  buf->bytes_used += sizeof(state);
  
  memcpy(&buf->buffer_data[buf->bytes_used], data, bytes_in_data);
  buf->bytes_used += bytes_in_data;
  
  return true;
}


} // anon ns


void
command_buffer_bind(Command_buffer *buffer, const Shader *shader)
{
  assert(buffer);
  
  insert_cmd(buffer,
             Detail::Buffer_state::set_shader,
             (void*)&shader->program_id,
             sizeof(shader->program_id));
}


} // ns