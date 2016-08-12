#include "command_buffer.hpp"
#include "clear.hpp"
#include "api_detail/command_state.hpp"
#include <assert.h>


namespace Graphics_api {


namespace {

// Various saved states.
  

} // anon ns


void
command_buffer_execute(Command_buffer *cmd)
{
  using namespace Graphics_api::Detail;

  size_t key_offset = 0;
  
  const uint8_t *cmds = cmd->buffer_data;
  
  while(key_offset < cmd->bytes_used)
  {
    const size_t data_offset = key_offset + sizeof(Buffer_state);
  
    switch(static_cast<Buffer_state>(cmds[key_offset]))
    {
      /*
        Clears the current target
      */
      case(Buffer_state::clear_target):
      {
        const Clear_flag::ENUM *clear_flags = reinterpret_cast<const Clear_flag::ENUM*>(&cmds[data_offset]);
        Graphics_api::clear(*clear_flags);
        
        key_offset += sizeof(Clear_flag::ENUM);
        
        break;
      }
      
      /*
        Sets the vertex format for the next draw call.
      */
      case(Buffer_state::set_vertex_attribute):
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