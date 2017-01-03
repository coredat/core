#ifndef RESOURCE_ID_INCLUDED_382773D6_0DBD_4846_80B6_E824345FC172
#define RESOURCE_ID_INCLUDED_382773D6_0DBD_4846_80B6_E824345FC172


#include <stdint.h>


namespace op {


struct resource_id
{
  uint32_t type : 8;
  uint32_t instance : 24;
};

} // ns

using opID = op::resource_id;


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


namespace op {

inline resource_id
invalid_resource_id()
{
  return resource_id {0,0};
}


inline bool
resource_is_valid(const resource_id id)
{
  return id.type > 0;
}


namespace resource_type {


enum ENUM : uint8_t
{
  INVALID,
  DEVICE,
  VERTEX_FORMAT,
  GEOMETRY,
  INDEX,
  SHADER,
  SHADER_DATA,
  TARGET,
  TEXTURE,
  TEXTURE_FILTER,
  RASTERIZER,
};


} // ns
} // ns


#endif // inc guard
