#ifndef CONTEXT_DATA_INCLUDED_FBB48592_1DBE_434A_A2FC_6F852A4007B9
#define CONTEXT_DATA_INCLUDED_FBB48592_1DBE_434A_A2FC_6F852A4007B9


#include <data/fwd_dec_data.hpp>
#include <op/fwd.hpp>
#include <memory>


namespace Data {


struct Context_data_setup
{
};


struct Context
{
  Font_data           *font_data          = nullptr;
  Font_glyph_data     *font_glyph_data    = nullptr;
  Material_data       *material_data      = nullptr;
  Mesh_data           *mesh_data          = nullptr;
  Post_process_data   *post_process_data  = nullptr;
  Shader_data         *shader_data        = nullptr;
  Text_mesh_data      *text_mesh_data     = nullptr;
  Texture_data        *texture_data       = nullptr;
  
  opContext           *op_context         = nullptr;
  opBuffer            *op_buffer          = nullptr;
  
  Context(const Context&)                 = delete;
  Context& operator=(const Context&)      = delete;
  
  explicit Context(const Context_data_setup &setup);
  
  ~Context();
};


/*
  Initialize context data.
*/
void
init_context_data(const Context_data_setup &setup);


/*
  Returns the context data.
*/
std::shared_ptr<Context>
get_context_data();


} // ns


#endif // inc guard