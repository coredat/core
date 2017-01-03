#ifndef FWD_INCLUDED_4A9C4647_4253_4032_A008_A8BDD6E8AA25
#define FWD_INCLUDED_4A9C4647_4253_4032_A008_A8BDD6E8AA25


#include <stdint.h>
#include <stddef.h>


namespace op {


// -- Interface -- //


class buffer;
class context;


// -- Callbacks -- //


using allocation_cb = void*(*)(size_t requested_size, uintptr_t user_data);
using resize_cb     = void*(*)(size_t requested_size, void *old_data, uintptr_t user_data);
using free_cb       = void(*)(void *data_to_destroy, uintptr_t user_data);


// -- Desc -- //


struct geometry_desc;
struct index_desc;
struct shader_desc;
struct shader_data_desc;
struct target_create_desc;
struct texture_desc;
struct texture_filter_desc;
struct vertex_format_desc;
struct vertex_attr;
struct rasterizer_desc;


} // ns


using allocCallback    = void*(*)(size_t requested_size, uintptr_t user_data);
using reallocCallback  = void*(*)(size_t requested_size, void *old_data, uintptr_t user_data);
using freeCallback     = void(*)(void *data_to_destroy, uintptr_t user_data);


struct opVertexDesc;
struct opShaderDesc;
struct opShaderDataDesc;
struct opGeometryDesc;
struct opIndexDesc;
struct opTargetDesc;
struct opTextureDesc;
struct opTextureFilterDesc;
struct opRasterizerDesc;


struct opBuffer;
struct opContext;


#endif // inc guard
