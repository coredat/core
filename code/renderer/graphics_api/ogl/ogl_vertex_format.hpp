#ifndef OGL_VERTEX_FORMAT_INCLUDED_2A5ADC43_7779_46FF_B20A_55C8CD781A97
#define OGL_VERTEX_FORMAT_INCLUDED_2A5ADC43_7779_46FF_B20A_55C8CD781A97


#include "ogl_common.hpp"
#include <stdint.h>
#include <stddef.h>


#define MAX_NUMBER_OF_ATTRS 8


namespace Ogl {


// TODO: This could move up a level?
enum class Attr_type
{
  float1,
  float2,
  float3,
  float4,
};


// TODO: Could this move up a level?
struct Attribute_desc
{
  const char  name[32];
  Attr_type   type;
};


struct Attribute
{
  const char    name[32];
  GLenum        type    = GL_NONE;
  uint32_t      size    = 0;
  size_t        pointer = 0;
};


struct Vertex_format
{
  Attribute   attributes[MAX_NUMBER_OF_ATTRS];
  uint32_t    stride               = 0;
  uint32_t    number_of_attributes = 0;
};


/*!
  Create an ogl vertex format.
  \param out_format is the result.
  \param attr_desc[] An array of vertex attributes.
  \param number_of_attributes How many attributes in the array.
*/
void
vertex_attributes_load(Vertex_format *out_format, Attribute_desc attr_desc[], const size_t number_of_attributes);


  
} // ns


#endif // inc guard