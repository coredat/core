#ifndef COMMON_INCLUDED_B1D69231_44E7_428F_AFFF_8E44F0B06C26
#define COMMON_INCLUDED_B1D69231_44E7_428F_AFFF_8E44F0B06C26

            /*
              Callbacks must be set before context and buffers are created.
              changing callbacks after is undefined.
            */

void        opCallbackAlloc();
void        opCallbackResize();
void        opCallbackDestroy();
void        opCallbackLog();

            // -- User Data -- //

void        opCallbackUserData();



#endif // inc guard
