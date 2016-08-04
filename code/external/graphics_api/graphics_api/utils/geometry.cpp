#include <graphics_api/utils/geometry.hpp>
#include <graphics_api/vertex_format.hpp>
#include <graphics_api/mesh.hpp>
#include <cstring>


namespace Graphics_api {


#ifndef UNREACHABLE
#define UNREACHABLE
#endif

#define GFX_MALLOC(size) malloc(size)
#define GFX_FREE(ptr) free(ptr)


Mesh
create_quads(const Vertex_format *fmt,
             const Quad_info *quad_info,
             const size_t number_of_quads)
{
  constexpr uint32_t pos_data_per_vertex = 3;
  constexpr float position_data[] =
  {
    -1.f, +1.f, 0.f,
    -1.f, -1.f, 0.f,
    +1.f, +1.f, 0.f,
    
    -1.f, -1.f, 0.f,
    +1.f, -1.f, 0.f,
    +1.f, +1.f, 0.f,
  };
  
  constexpr uint32_t texture_data_per_vertex = 2;
  constexpr float texture_coord_data[] =
  {
    0.f, 1.f,
    0.f, 0.f,
    1.f, 1.f,
    
    0.f, 0.f,
    1.f, 0.f,
    1.f, 1.f,
  };
  
  constexpr uint32_t normal_data_per_vertex = 3;
  constexpr float normal_data[] =
  {
    0.f, 0.f, +1.f,
    0.f, 0.f, +1.f,
    0.f, 0.f, +1.f,
    
    0.f, 0.f, +1.f,
    0.f, 0.f, +1.f,
    0.f, 0.f, +1.f,
  };
  
  // Format desc
  Vertex_attribute desc[vertex_format_get_max_number_attributes()];
  memset(desc, 0, sizeof(desc));
  
  vertex_format_get_desc(fmt, desc);
  
  const uint32_t attr_count = vertex_format_get_number_of_attributes(fmt);
  
  uint32_t buffer_items = 0;
  // Need to calculate the size of the buffer!
  for(uint32_t i = 0; i < attr_count; ++i)
  {
    switch(desc[i])
    {
      case(Vertex_attribute::position_3d):
        buffer_items += 3;
        break;
      case(Vertex_attribute::position_2d):
        buffer_items += 2;
        break;
      case(Vertex_attribute::normal):
        buffer_items += 3;
        break;
      case(Vertex_attribute::texture_coord):
        buffer_items += 2;
        break;
      default:
        assert(false);
    }
    UNREACHABLE;
  }
  
  const size_t buffer_size = sizeof(float) * (buffer_items * number_of_quads);
  float *buffer_data = (float*)GFX_MALLOC(buffer_size);
  uint32_t buffer_offset = 0;
  
  const uint32_t number_of_attributes = vertex_format_get_number_of_attributes(fmt);

  for(size_t i = 0; i < number_of_quads; ++i)
  {
    constexpr uint32_t verts_in_quad = 6;
  
    for(uint32_t j = 0; j < number_of_attributes * verts_in_quad; ++j)
    {
      const uint32_t current_vert = j % verts_in_quad;
    
      switch(desc[i])
      {
        case(Vertex_attribute::position_3d):
          memcpy(&buffer_data[buffer_offset], &position_data[current_vert * pos_data_per_vertex], sizeof(float) * 3);
          buffer_data += pos_data_per_vertex;
          break;
          
        case(Vertex_attribute::position_2d):
          memcpy(&buffer_data[buffer_offset], &position_data[current_vert * pos_data_per_vertex], sizeof(float) * 2);
          buffer_data += 2; // Special case as we don't want z value.
          break;
          
        case(Vertex_attribute::normal):
          memcpy(&buffer_data[buffer_offset], &normal_data[current_vert * normal_data_per_vertex], sizeof(float) * 3);
          buffer_data += normal_data_per_vertex;
          break;
          
        case(Vertex_attribute::texture_coord):
          memcpy(&buffer_data[buffer_offset], &texture_coord_data[current_vert * texture_data_per_vertex], sizeof(float) * 2);
          buffer_data += texture_data_per_vertex;
          break;
          
        default:
          assert(false);
      }
    }
  }
  
  // Create vbo
  Mesh return_mesh;
  Graphics_api::mesh_create_new(&return_mesh, buffer_data, number_of_attributes);
  
  GFX_FREE(buffer_data);
  
  return return_mesh;
}


} // ns