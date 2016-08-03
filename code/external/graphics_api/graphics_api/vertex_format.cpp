#include "vertex_format.hpp"


#define VF_NAME "in_vs_position"
#define VF_NORMAL "in_vs_normal"
#define VF_TEX_COORD "in_vs_texture_coord"


namespace Graphics_api {


Vertex_format
vertex_format_create(const Vertex_attribute desc[],
                     const uint32_t attribute_count)
{
  // Only support 16 attributes.
  Ogl::Attribute_desc ogl_vert_attr[16];
  assert(attribute_count < 16);
  
  bool has_position       = false;
  bool has_normal         = false;
  bool has_texture_coord  = false;
  
  for(uint32_t i = 0; i < attribute_count; ++i)
  {
    switch(desc[i])
    {
      case(Vertex_attribute::position_3d):
      {
        assert(!has_position); // Can only have one pos attr
        has_position = true;
        ogl_vert_attr[i] = {VF_NAME, Ogl::Attr_type::FLOAT3};
        break;
      }
      
      case(Vertex_attribute::position_2d):
      {
        assert(!has_position); // Can only have one pos attr
        has_position = true;
        ogl_vert_attr[i] = {"in_vs_position", Ogl::Attr_type::FLOAT2};
        break;
      }
      
      case(Vertex_attribute::normal):
      {
        assert(!has_normal); // Can only have one normal
        has_normal = true;
        ogl_vert_attr[i] = {VF_TEX_COORD, Ogl::Attr_type::FLOAT3};
        break;
      }
      
      case(Vertex_attribute::texture_coord):
      {
        assert(!has_texture_coord); // Can only have one tex coord
        has_texture_coord = true;
        ogl_vert_attr[i] = {"in_vs_texture_coord", Ogl::Attr_type::FLOAT2};
        break;
      }
      
      default:
        assert(false); // Unkown attr.
    };
  }
  
  Vertex_format vert_fmt;
  Ogl::vertex_format_load(&vert_fmt.format, ogl_vert_attr, attribute_count);
  assert(Ogl::vertex_format_is_valid(&vert_fmt.format));
  
  return vert_fmt;
}


uint32_t
vertex_format_get_number_of_attributes(const Vertex_format *fmt)
{
  assert(fmt);
  return fmt->format.number_of_attributes;
}


void
vertex_format_get_desc(const Vertex_format *fmt,
                       Vertex_attribute *out_desc)
{
  const uint32_t attr_count = fmt->format.number_of_attributes;
  
  for(uint32_t i = 0; i < attr_count; ++i)
  {
    // Is position?
    if(strcmp(fmt->format.attributes->name, "in_vs_position") == 0)
    {
      
    }
    
    // Normal
    else if(strcmp(fmt->format.attributes->name, "in_vs_normal") == 0)
    {
    }
    
    //
    else if(strcmp(fmt->format.attributes->name, "in_vs_texture_coord") == 0)
    {
    
    }
  }
}


} // ns