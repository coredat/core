#ifndef TODO_ONCE_FIND_HOME_FOR_THESE
#define TODO_ONCE_FIND_HOME_FOR_THESE

#include <utilities/generic_id.hpp>

namespace Data {

struct Mesh_renderer_draw_call {
  util::generic_id model_id;
  float world_matrix[16];
};

}


#endif // inc guard