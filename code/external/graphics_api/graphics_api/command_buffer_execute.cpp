#include "command_buffer.hpp"
#include "api_detail/command_state.hpp"
#include <assert.h>


namespace Graphics_api {


namespace {

// Various saved states.

} // anon ns


void
command_buffer_execute(Command_buffer *cmd)
{
  size_t offset = 0;
  
  const uint8_t *cmds = cmd->buffer_data;
  
  while(offset < cmd->bytes_used)
  {
    using namespace Graphics_api::Detail;
    
    switch(static_cast<Buffer_state>(cmds[offset]))
    {
      /*
        Clears the current target
      */
      case(Buffer_state::clear_target):
      {
        break;
      }
      
      /*
        Sets the vertex format for the next draw call.
      */
      case(Buffer_state::set_vertex_format):
      {
        break;
      }
      
      /*
        Sets the vbo and ibo for next draw call.
      */
      case(Buffer_state::set_geometry):
      {
        break;
      }
      
      /*
        Sets the current shader.
      */
      case(Buffer_state::set_shader):
      {
        break;
      }
      
      /*
        Draw call, uses all the set states to do a draw call
      */
      case(Buffer_state::draw_call):
      {
        break;
      }
      
      /*
        
      */
      
      default:
        // All states should be handeled.
        assert(false);
    }
  }
}


} // ns