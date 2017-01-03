#include "../../include/op/context.hpp"
#include "../data/context_data.hpp"
#include "../common/common.hpp"
#include "context_impl.hpp"
#include "common_impl.hpp"


// -- Context Management -- //


opContext*
opContextCreate()
{
  opContext *ctx = new opContext;

  ctx->data.alloc_cb    = getCallbackAlloc();
  ctx->data.realloc_cb  = getCallbackRealloc();
  ctx->data.destroy_cb  = getCallbackFree();
  ctx->data.user_data   = getCallbackUserData();;

  ctx->data.initial_reserve();

  return ctx;
}


// -- Context Support -- //


uint8_t
opContext1DTextureSupport(const opContext* ctx)
{
  return 0;
}


uint8_t
opContext3DTextureSupport(const opContext* ctx)
{
  return 0;
}


uint32_t
opContext1DTextureMaxWidth(const opContext* ctx)
{
  return 0;
}


uint32_t
opContext2DTextureMaxWidth(const opContext* ctx)
{
  return 0;
}


uint32_t
opContext2DTextureMaxHeight(const opContext* ctx)
{
  return 0;
}


uint32_t
opContext3DTextureMaxWidth(const opContext* ctx)
{
  return 0;
}


uint32_t
opContext3DTextureMaxHeight(const opContext* ctx)
{
  return 0;
}


uint32_t
opContext3DTextureMaxDepth(const opContext* ctx)
{
  return 0;
}


uint8_t
opContextAnisotropicSupport(const opContext* ctx)
{
  return 0;
}


float
opContextAnisotropicMax(const opContext* ctx)
{
  return 0.f;
}
