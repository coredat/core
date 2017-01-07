#ifndef DESC_INCLUDED_5EE4A281_71DD_424B_BA37_B978549912CA
#define DESC_INCLUDED_5EE4A281_71DD_424B_BA37_B978549912CA


#include <stdint.h>


struct opShaderDesc
{
  uint8_t  status;                // OUT
  uint16_t number_of_samplers;    // OUT
  uint16_t number_of_uniforms;    // OUT
  uint16_t number_of_outputs;     // OUT
};


struct opVertexDesc
{
  const char * name;              // IN
  uint8_t type;                   // IN
};


struct opTextureDesc
{
  uint8_t status;                 // OUT

  uint16_t width;                 // IN
  uint16_t height;                // IN
  uint16_t depth;                 // IN

  uint8_t dimention;              // IN_OUT
  uint8_t format;                 // IN_OUT

  uint8_t mips;                   // IN

  uint8_t filter;                 // IN_OUT
  uint8_t filter_multiplier;      // IN_OUT

  uint8_t wrap_mode_width;        // IN
  uint8_t wrap_mode_height;       // IN
  uint8_t wrap_mode_depth;        // IN

  uintptr_t platform_id;          // OUT
};


struct opShaderDataDesc
{
  uint8_t status;                 // OUT
  uint8_t data_type;              // OUT
  uint16_t count;                 // OUT
};


struct opGeometryDesc
{
  uint8_t status;                 // OUT
  uint8_t access;                 // IN
};


struct opIndexDesc
{
  uint8_t status;                 // OUT
  uint8_t access;                 // IN
};


struct opRasterizerDesc
{
  uint8_t cull_face;              // IN
  uint8_t winding_order;          // IN
  uint8_t primitive;              // IN
};


struct opTargetDesc
{
  uint8_t   pixel_format;         // IN_OUT
  uint32_t  width;                // IN
  uint32_t  height;               // IN

  float     clear_red_color;      // IN
  float     clear_green_color;    // IN
  float     clear_blue_color;     // IN
};


#endif // inc guard
