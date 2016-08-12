#ifndef COMMAND_BUFFER_INCLUDED_6AA17ABC_3BBB_43A7_A726_F2C73D59CBCA
#define COMMAND_BUFFER_INCLUDED_6AA17ABC_3BBB_43A7_A726_F2C73D59CBCA


#include "gfx_common.hpp"
#include "gfx_fwd.hpp"


namespace Graphics_api {


struct Command_buffer
{
  uint8_t       *buffer_data = nullptr;
  
  size_t        bytes_used = 0;
  const size_t  bytes_capacity = 0;
};


void
command_buffer_create(Command_buffer *bufffer,
                      const size_t capacity_hint);


void
command_buffer_destroy(Command_buffer *buffer);


void
command_buffer_bind(Command_buffer *buffer,
                    const Shader *shader);
  
  
void
command_buffer_bind(Command_buffer *buffer,
                    const Vertex_format *vert_fmt);


void
command_buffer_bind(Command_buffer *buffer,
                    const Texture *texture);


void
command_buffer_bind(Command_buffer *buffer,
                    const Mesh *mesh);


void
command_buffer_bind(Command_buffer *buffer,
                    const Uniform *uni);


void
command_buffer_draw(Command_buffer *buffer);


void
command_buffer_execute(Command_buffer *buffer);


} // ns


#endif // inc guard