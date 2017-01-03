#ifndef CONTEXT_INCLUDED_DED35F68_9BE1_4F4B_A728_7AAF48686BCD
#define CONTEXT_INCLUDED_DED35F68_9BE1_4F4B_A728_7AAF48686BCD


#include "fwd.hpp"
#include <stdint.h>

            // -- Context Management -- //

opContext*  opContextCreate();

            // -- Context Support -- //

uint8_t     opContext1DTextureSupport(const opContext* ctx);
uint8_t     opContext3DTextureSupport(const opContext* ctx);

uint32_t    opContext1DTextureMaxWidth(const opContext* ctx);
uint32_t    opContext2DTextureMaxWidth(const opContext* ctx);
uint32_t    opContext2DTextureMaxHeight(const opContext* ctx);
uint32_t    opContext3DTextureMaxWidth(const opContext* ctx);
uint32_t    opContext3DTextureMaxHeight(const opContext* ctx);
uint32_t    opContext3DTextureMaxDepth(const opContext* ctx);

uint8_t     opContextAnisotropicSupport(const opContext* ctx);
float       opContextAnisotropicMax(const opContext* ctx);


void        opContextAllocCallback(const allocCallback cb);
void        opContextResizeCallback(const reallocCallback cb);
void        opContextDestroyCallback(const freeCallback cb);
void        opContextUserData(const uintptr_t user_data);


#endif // inc guard
