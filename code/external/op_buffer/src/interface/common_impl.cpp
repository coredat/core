#include "common_impl.hpp"


namespace {


allocCallback callback_alloc_fn     = nullptr;
reallocCallback callback_realloc_fn = nullptr;
freeCallback callback_free_fn       = nullptr;
logCallback callback_log_fn         = nullptr;
uintptr_t user_data = 0;


} // anon ns


allocCallback
getCallbackAlloc()
{
  return callback_alloc_fn;
}

reallocCallback
getCallbackRealloc()
{
  return callback_realloc_fn;
}

freeCallback
getCallbackFree()
{
  return callback_free_fn;
}

logCallback
getCallbackLog()
{
  return callback_log_fn;
}

uintptr_t
getCallbackUserData()
{
  return user_data;
}

void
setCallbackAlloc(allocCallback cb)
{
  callback_alloc_fn = cb;
}

void
setCallbackRealloc(reallocCallback cb)
{
  callback_realloc_fn = cb;
}

void
setCallbackFree(freeCallback cb)
{
  callback_free_fn = cb;
}

void
setCallbackLog(logCallback cb)
{
  callback_log_fn = cb;
}

void
setCallbackUserData(const uintptr_t ud)
{
  user_data = ud;
}
