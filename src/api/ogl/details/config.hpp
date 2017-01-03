#ifdef OP_BUFFER_API_OGL
#ifndef CONFIG_INCLUDED_F1C074A8_8EAF_4274_9E08_E02611469E9D
#define CONFIG_INCLUDED_F1C074A8_8EAF_4274_9E08_E02611469E9D


#include <stdint.h>


namespace op {
namespace ogl {


constexpr uint16_t max_attr_name_length()     { return 32; }
constexpr uint16_t max_vertex_attrs()         { return 8;  }
constexpr uint16_t max_uniform_name_length()  { return 32; }


} // ns
} // ns


#endif // inc guard
#endif // api guard
