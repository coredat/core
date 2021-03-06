#ifndef BUFFER_INCLUDED_FF652886_7174_4B7D_9131_9D6FFB784106
#define BUFFER_INCLUDED_FF652886_7174_4B7D_9131_9D6FFB784106


#include "fwd.hpp"
#include <stdint.h>


            // -- Buffer Management -- //

opBuffer*   opBufferCreate(const size_t initialize_size);
void        opBufferExec(opContext *ctx, opBuffer *buf);
void        opBufferFlush(opBuffer *buf);
void        opBufferReset(opBuffer *buf);

            // -- Device -- //

opID        opBufferDeviceCreate(opContext *ctx, opBuffer *buf);
void        opBufferDeviceReset(opBuffer *buf);

            // -- Vertex Format -- //

opID        opBufferVertexFormatCreate(opContext *ctx, opBuffer *buf, opVertexDesc desc[], const size_t attr_count);
void        opBufferVertexFormatBind(opBuffer *buf, const opID id);

            // -- Shaders -- //

opID        opBufferShaderCreate(opContext *ctx, opBuffer *buf, const char *name, const char *vs, const char *gs, const char *ps, opShaderDesc *in_out_desc);
void        opBufferShaderBind(opBuffer *buffer, const opID id);

            // -- Shader Data -- //

opID        opBufferShaderDataCreate(opContext *ctx, opBuffer *buf, const opID shader_id, const char *name, opShaderDataDesc *in_out_desc);
void        opBufferShaderDataBind(opBuffer *buf, const opID id, void *data);
void        opBufferShaderDataBind(opBuffer *buf, const opID id, const opID texture_id);

            // -- Geometry -- //

opID        opBufferGeometryCreate(opContext *ctx, opBuffer *buf, void *data, const size_t bytes_of_data, const size_t element_count, opGeometryDesc *in_out_desc);
void        opBufferGeometryBind(opBuffer *buf, const opID id);

            // -- Index -- //

opID        opBufferIndexCreate(opContext *ctx, opBuffer *buf, void *data, const size_t bytes_of_data, const size_t element_count, opIndexDesc *in_out_desc);
void        opBufferIndexBind(opBuffer *buf, const opID id);

            // -- Target -- //

opID        opBufferTargetCreate(opContext *ctx, opBuffer *buf, opTargetDesc *in_out_desc);
void        opBufferTargetBind(opBuffer *buf, const opID id);
void        opBufferTargetClear(opBuffer *buf, const opID id, const bool color, const bool depth);

            // -- Texture -- //

opID        opBufferTextureCreate(opContext *ctx, opBuffer *buf, void *data, opTextureDesc *in_out_desc);
void        opBufferTextureUpdate(opContext *ctx, opBuffer *buf, const opID id, const size_t offset_x, const size_t width, void *data);
void        opBufferTextureUpdate(opContext *ctx, opBuffer *buf, const opID id, const size_t offset_x, const size_t offset_y, const size_t width, const size_t height, void *data);
void        opBufferTextureUpdate(opContext *ctx, opBuffer *buf, const opID id, const size_t offset_x, const size_t offset_y, const size_t offset_z, const size_t width, const size_t height, const size_t depth, void *data);
void        opBufferTextureBind(opBuffer *buf, const opID id);

            // -- Rasterizer -- //

opID        opBufferRasterizerCreate(opContext *ctx, opBuffer *buf, opRasterizerDesc *in_out_desc);
void        opBufferRasterizerBind(opBuffer *buf, const opID id);

            // -- Blending -- //

opID        opBufferBlendCreate(opContext *ctx, opBuffer *buf, opBlendDesc *in_out_desc);
void        opBufferBlendBind(opBuffer *buf, const opID id);

            // -- Render -- //

void        opBufferRender(opBuffer *buf);
void        opBufferRenderSubset(opBuffer *buf, const uint32_t start_vert, const uint32_t end_vert);

            // -- Misc -- //

void        opBufferViewport(opBuffer *buf, const size_t start_x, const size_t start_y, const size_t end_x, const size_t end_y);

            // -- Debug -- //

void        opBufferDebugMarkerPush(opBuffer *buf, const char *name);
void        opBufferDebugMarkerPop(opBuffer *buf);

            // -- Benchmark -- //

opID        opBufferBenchmarkCreate(opContext *ctx, opBuffer *buf, void *desc);
void        opBufferBenchmarkStart(opBuffer *buf, const opID id);
void        opBufferBenchmarkEnd(opBuffer *buf, const opID id);


#endif // inc guard
