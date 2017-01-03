#ifndef COMMON_INCLUDED_B1D69231_44E7_428F_AFFF_8E44F0B06C26
#define COMMON_INCLUDED_B1D69231_44E7_428F_AFFF_8E44F0B06C26


#include <stdint.h>
#include "fwd.hpp"


/*
  Callbacks must be set before context and buffers are created.
  changing callbacks after is undefined.
*/

            // -- Callbacks -- //

void        opCallbackAlloc(const allocCallback cb);
void        opCallbackResize(const reallocCallback cb);
void        opCallbackDestroy(const freeCallback cb);
void        opCallbackLog(const logCallback cb);

            // -- User Data -- //

void        opCallbackUserData(const uintptr_t user_data);


#endif // inc guard
