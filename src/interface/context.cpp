#include "../../include/op/context.hpp"
#include "../data/context_data.hpp"
#include "../common/common.hpp"
#include "context_impl.hpp"


namespace {

allocCallback alloc_cb     = nullptr;
reallocCallback realloc_cb = nullptr;
freeCallback free_cb       = nullptr;
uintptr_t user_data = 0;

} // anon ns


opContext*
opContextCreate()
{
  opContext *ctx = new opContext;

  ctx->data.alloc_cb    = alloc_cb;
  ctx->data.realloc_cb  = realloc_cb;
  ctx->data.destroy_cb  = free_cb;
  ctx->data.user_data   = user_data;
  ctx->data.initial_reserve();

  return ctx;
}


void
opContextAllocCallback(const allocCallback cb)
{
  alloc_cb = cb;
}


void
opContextResizeCallback(const reallocCallback cb)
{
  realloc_cb = cb;
}


void
opContextDestroyCallback(const freeCallback cb)
{
  free_cb = cb;
}


void
opContextUserData(const uintptr_t ud)
{
  user_data = ud;
}
