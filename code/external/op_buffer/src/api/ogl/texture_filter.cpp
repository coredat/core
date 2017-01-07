// #ifdef OP_BUFFER_API_OGL
//
//
// #include "texture_filter.hpp"
// #include "details/ogl_common.hpp"
// #include "details/error_code_to_text.hpp"
// #include "details/enum_to_string.hpp"
// #include "details/op_common.hpp"
// #include "details/op_to_ogl.hpp"
// #include "descs/descs.hpp"
//
//
// namespace op {
// namespace ogl {
//
//
// void
// texture_filter_create(context_data *context, void *data)
// {
//   // -- Param Check -- //
//   assert(context);
//   assert(data);
//
//   // -- Get Cmd Data -- //
//   command::cmd_texture_filter_create *cmd(
//     reinterpret_cast<command::cmd_texture_filter_create*>(data)
//   );
//
//   // -- Get Texture Filter Desc -- //
//   ogl::texture_filtering_internal_desc *internal_desc(
//     &context->texture_filtering_descs[cmd->texture_filter_id]
//   );
//   assert(internal_desc);
//
//   // -- Generate Filter -- //
//   internal_desc->filter_mode       = cmd->desc->filter_mode;
//   internal_desc->filter_multiplier = cmd->desc->filter_multiplier;
//   internal_desc->wrap_mode_width   = cmd->desc->wrap_mode_width;
//   internal_desc->wrap_mode_height  = cmd->desc->wrap_mode_height;
//   internal_desc->wrap_mode_depth   = cmd->desc->wrap_mode_depth;
//
//   #ifdef OP_BUFFER_LOG_INFO
//   context->log("[");
//   context->log("  Texture Filter Created");
//   context->log("  --");
//   context->log("  Actual GL Settings are deffered until bind");
//   context->log("]");
//   #endif
// }
//
//
// void
// texture_filter_update(context_data *context, void *data)
// {
//
// }
//
//
// void
// texture_filter_destroy(context_data *context, void *data)
// {
//
// }
//
//
// void
// texture_filter_bind(context_data *context, void *data)
// {
//
// }
//
//
// } // ns
// } // ns
//
//
// #endif // inc guard
