#include "../../include/op/common.hpp"
#include "common_impl.hpp"


void
opCallbackAlloc(const allocCallback cb)
{
  setCallbackAlloc(cb);
}


void
opCallbackResize(const reallocCallback cb)
{
  setCallbackRealloc(cb);
}


void
opCallbackDestroy(const freeCallback cb)
{
  setCallbackFree(cb);
}


void
opCallbackLog(const logCallback cb)
{
  setCallbackLog(cb);
}


void
opCallbackUserData(const uintptr_t user_data)
{
  setCallbackUserData(user_data);
}
