#include <graphics_api/utils/geometry.hpp>
#include <graphics_api/vertex_format.hpp>
#include <graphics_api/mesh.hpp>


namespace Graphics_api {


Mesh
create_quads(const Vertex_format *fmt,
             const Quad_info *quad_info,
             const size_t number_of_quads)
{
  constexpr float quad_data[] =
  {
    -1.f, +1.f, 0.f, 0.f, 1.f,
    -1.f, -1.f, 0.f, 0.f, 0.f,
    +1.f, +1.f, 0.f, 1.f, 1.f,
    
    -1.f, -1.f, 0.f, 0.f, 0.f,
    +1.f, -1.f, 0.f, 1.f, 0.f,
    +1.f, +1.f, 0.f, 1.f, 1.f,
  };
  
  // Need to calculate the size of the buffer!
    
  
  const uint32_t number_of_attributes = 3;

  for(size_t i = 0; i < number_of_quads; ++i)
  {
    for(uint32_t j = 0; j < number_of_attributes; ++j)
    {
    }
  }
}


} // ns