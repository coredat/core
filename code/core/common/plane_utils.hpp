#ifndef PLANE_UTILS_INCLUDED_5F59ECE9_C364_4425_A3D6_6A5B22265377
#define PLANE_UTILS_INCLUDED_5F59ECE9_C364_4425_A3D6_6A5B22265377


#include <core/common/core_fwd.hpp>


namespace Core {
namespace Plane_utils {


bool
ray_intersects_with_plane(const Plane &plane, const Ray &ray, float &out_distance);


} // ns
} // ns


#endif // inc guard