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
  LOG_TODO_ONCE("Actually this would be nicer to get a a void* of data back wouldn't it?");

  constexpr uint32_t pos3d_data_per_vertex = 3;
  constexpr uint32_t pos2d_data_per_vertex = 2;
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
        buffer_items += pos3d_data_per_vertex;
        break;
        
      case(Vertex_attribute::position_2d):
        buffer_items += pos2d_data_per_vertex;
        break;
        
      case(Vertex_attribute::normal):
        buffer_items += normal_data_per_vertex;
        break;
        
      case(Vertex_attribute::texture_coord):
        buffer_items += texture_data_per_vertex;
        break;
        
      default:
        assert(false);
    }
    UNREACHABLE;
  }
  
  constexpr uint32_t verts_in_quad = 6;
  const uint32_t number_of_attributes = vertex_format_get_number_of_attributes(fmt);

  const size_t number_of_entries = verts_in_quad * buffer_items * number_of_quads;
  const size_t buffer_size = sizeof(float) * (number_of_entries);
  
  float *buffer_data = (float*)GFX_MALLOC(buffer_size);
  uint32_t buffer_offset = 0;

  for(size_t i = 0; i < number_of_quads; ++i) {
    for(uint32_t j = 0; j < verts_in_quad; ++j) {
      for(uint32_t k = 0; k < 3; ++k)
      {
        const uint32_t current_vert = j % verts_in_quad;
        
        switch(desc[k])
        {
          case(Vertex_attribute::position_3d):
            memcpy(&buffer_data[buffer_offset], &position_data[current_vert * pos3d_data_per_vertex], sizeof(float) * pos3d_data_per_vertex);
            buffer_offset += pos3d_data_per_vertex;
            break;
            
          case(Vertex_attribute::position_2d):
            // We need to use pos3d_data_per_vertex to get the data, as the data is 3d.
            memcpy(&buffer_data[buffer_offset], &position_data[current_vert * pos3d_data_per_vertex], sizeof(float) * pos2d_data_per_vertex);
            buffer_offset += pos2d_data_per_vertex;
            break;
            
          case(Vertex_attribute::normal):
            memcpy(&buffer_data[buffer_offset], &normal_data[current_vert * normal_data_per_vertex], sizeof(float) * normal_data_per_vertex);
            buffer_offset += normal_data_per_vertex;
            break;
            
          case(Vertex_attribute::texture_coord):
            memcpy(&buffer_data[buffer_offset], &texture_coord_data[current_vert * texture_data_per_vertex], sizeof(float) * texture_data_per_vertex);
            buffer_offset += texture_data_per_vertex;
            break;
            
          default:
            assert(false);
        }
      }
    }
  }
  
  // Create vbo
  Mesh return_mesh;
  Graphics_api::mesh_create_new(&return_mesh, buffer_data, number_of_attributes * verts_in_quad * number_of_quads);
  
//  delete[] buffer_data; //(buffer_data);
  GFX_FREE(buffer_data);
  
  return return_mesh;
}


} // ns