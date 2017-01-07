#ifndef CONTEXT_INCLUDED_DED35F68_9BE1_4F4B_A728_7AAF48686BCD
#define CONTEXT_INCLUDED_DED35F68_9BE1_4F4B_A728_7AAF48686BCD


#include "fwd.hpp"
#include <stdint.h>

            // -- Context Management -- //

opContext*  opContextCreate();

            // -- Context Stats -- //

void        opContextResetStats(opContext *ctx);
size_t      opContextDrawCallStats(const opContext *ctx);

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


#endif // inc guard
