#ifndef COMMON_IMPL_B25BB145_D68B_4F93_9B8D_6E6EC765F0B1
#define COMMON_IMPL_B25BB145_D68B_4F93_9B8D_6E6EC765F0B1


#include <stdint.h>
#include "../../include/op/fwd.hpp"


allocCallback
getCallbackAlloc();

reallocCallback
getCallbackRealloc();

freeCallback
getCallbackFree();

logCallback
getCallbackLog();

uintptr_t
getCallbackUserData();

void
setCallbackAlloc(allocCallback cb);

void
setCallbackRealloc(reallocCallback cb);

void
setCallbackFree(freeCallback cb);

void
setCallbackLog(logCallback cb);

void
setCallbackUserData(const uintptr_t user_data);


#endif // inc guard
