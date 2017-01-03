#ifndef FWD_INCLUDED_4A9C4647_4253_4032_A008_A8BDD6E8AA25
#define FWD_INCLUDED_4A9C4647_4253_4032_A008_A8BDD6E8AA25


#include <stdint.h>
#include <stddef.h>


// -- Callbacks -- //

using allocCallback    = void*(*)(size_t requested_size, uintptr_t user_data);
using reallocCallback  = void*(*)(size_t requested_size, void *old_data, uintptr_t user_data);
using freeCallback     = void(*)(void *data_to_destroy, uintptr_t user_data);
using logCallback      = void(*)(const char *log, uintptr_t user_data);

// -- Resource Descs -- //

struct opVertexDesc;
struct opShaderDesc;
struct opShaderDataDesc;
struct opGeometryDesc;
struct opIndexDesc;
struct opTargetDesc;
struct opTextureDesc;
struct opTextureFilterDesc;
struct opRasterizerDesc;

// -- Objects -- //

struct opBuffer;
struct opContext;


#endif // inc guard
