#include "command_buffer.hpp"
#include "api_detail/command_state.hpp"
#include "shader.hpp"
#include "texture.hpp"
#include "mesh.hpp"
#include "vertex_format.hpp"
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


namespace Gfx_detail = ::Graphics_api::Detail;


inline bool
insert_cmd(Graphics_api::Command_buffer *buf,
           const Gfx_detail::Buffer_state state,
           const void *data,
           const size_t bytes_in_data)
{
  assert(buf);
  
  const size_t bytes_needed = sizeof(Gfx_detail::Buffer_state) + bytes_in_data;
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
command_buffer_bind(Command_buffer *buffer,
                    const Shader *shader)
{
  assert(buffer);
  
  Gfx_detail::Bind_shader shader_data;
  shader_data.shader_id = shader->program_id;
  
  insert_cmd(buffer,
             Gfx_detail::Buffer_state::set_shader,
             &shader_data,
             sizeof(shader_data));
}


void
command_buffer_bind(Command_buffer *buffer,
                    const Vertex_format *vert_fmt)
{
  // Loop through and att and attribute for each vertex formats
  for(uint32_t i = 0; i < vert_fmt->format.number_of_attributes; ++i)
  {
    const auto *curr_attr = &vert_fmt->format.attributes[i];
  
    Gfx_detail::Bind_vertex_attr attr;
    
    memset(attr.name, 0, sizeof(attr.name));
    strcpy(attr.name, curr_attr->name);
    
    attr.type    = curr_attr->type;
    attr.size    = curr_attr->size;
    attr.pointer = curr_attr->pointer;
    
    
    insert_cmd(buffer,
               Gfx_detail::Buffer_state::set_vertex_attribute,
               &attr,
               sizeof(attr));
  }
}


void
command_buffer_bind(Command_buffer *buffer,
                    const Mesh *mesh)
{
  Gfx_detail::Bind_mesh mesh_data;
  mesh_data.vbo_id      = mesh->vbo.vertex_buffer_id;
  mesh_data.vbo_entries = mesh->vbo.number_of_entries;
  
  insert_cmd(buffer,
             Gfx_detail::Buffer_state::set_geometry,
             &mesh_data,
             sizeof(mesh_data));
}



} // ns