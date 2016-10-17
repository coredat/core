#ifndef TODO_ONCE_
#define TODO_ONCE_

#include <utilities/generic_id.hpp>

namespace Data {

namespace Entity_component {
  enum ENUM {
    has_physics = 1 << 0,
  };
} // ns


struct Mesh_renderer_draw_call {
  util::generic_id model_id;
  float world_matrix[16];
};

}


#endif // inc guard