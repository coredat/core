#ifdef OP_BUFFER_API_OGL
#ifndef VERTEX_FORMAT_INCLUDED_BD38F98C_2252_41DB_8A80_349F0C392AA5
#define VERTEX_FORMAT_INCLUDED_BD38F98C_2252_41DB_8A80_349F0C392AA5


#include "../../data/fwd.hpp"


namespace op {
namespace ogl {


void
vertex_fmt_create(context_data *context, void *data);


void
vertex_fmt_destroy(context_data *context, void *data);


void
vertex_fmt_bind(context_data *context, void *data);


} // ns
} // ns


#endif // inc guard
#endif // api guard
