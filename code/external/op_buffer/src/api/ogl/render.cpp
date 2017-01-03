#ifdef OP_BUFFER_API_OGL


#include "render.hpp"
#include "details/ogl_common.hpp"
#include "details/error_code_to_text.hpp"
#include "details/op_common.hpp"
#include "details/op_to_ogl.hpp"
#include "descs/descs.hpp"


namespace op {
namespace ogl {


void
render_generic(context_data *context, void *data)
{
  // -- Param Check -- //
  assert(context);
  assert(data);

  GLenum primitive = GL_TRIANGLES;

  if(context->rasterizer_currently_bound)
  {
    const ogl::rasterizer_desc *rasterizer_desc(
      &context->rasterizer_descs[context->rasterizer_last_bind]
    );

    primitive = rasterizer_desc->primitive;
  }

  // -- Render Index -- //
  if(context->index_currently_bound && context->geometry_currently_bound)
  {
    const ogl::index_internal_desc *index_desc(
      &context->index_descs[context->index_last_bind]
    );

    glDrawElements(
      primitive,
      index_desc->element_count,
      GL_UNSIGNED_INT,
      0
    );
  }

  // -- Render Non Indexd -- //
  else if(context->geometry_currently_bound && context->vertex_format_currently_bound)
  {
    const ogl::geometry_desc *geom_desc(
      &context->geometry_descs[context->geometry_last_bind]
    );

    const ogl::vertex_format_desc *vert_desc(
      &context->vertex_format_descs[context->vertex_format_last_bind]
    );

    const GLsizei count = geom_desc->element_count / vert_desc->number_of_attributes;

    glDrawArrays(primitive, 0, count);
  }

  // -- Can't Render -- //
  #ifdef OP_BUFFER_LOG_INFO
  else
  {
    context->log("Logic Error - No VBO/IBO has been bound");
  }
  #endif

  // -- Extra Check -- //
  #ifdef OP_BUFFER_API_OGL_EXTRA_CHECKS
  const GLenum err_code = glGetError();

  if(err_code)
  {
    context->log(
      "GL Error - Draw Arrays %d : %s",
      err_code,
      get_error_msg(err_code)
    );
  }
  #endif
}


void
render_subset(context_data *context, void *data)
{

}


} // ns
} // ns


#endif // api guard
