#ifndef RESOURCE_ID_INCLUDED_382773D6_0DBD_4846_80B6_E824345FC172
#define RESOURCE_ID_INCLUDED_382773D6_0DBD_4846_80B6_E824345FC172


#include <stdint.h>
#include "../fwd.hpp"


bool      opIDIsValid(const opID id);
bool      opIDIsDevice(const opID id);
bool      opIDIsVertexFormat(const opID id);
bool      opIDIsGeometry(const opID id);
bool      opIDIsIndex(const opID id);
bool      opIDIsShader(const opID id);
bool      opIDIsShaderData(const opID id);
bool      opIDIsTarget(const opID id);
bool      opIDIsTexture(const opID id);
bool      opIDIsTextureFilter(const opID id);
bool      opIDIsRasterizer(const opID id);


#endif // inc guard
