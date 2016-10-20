#ifndef ENTITY_COMPONENTS_INCLUDED_C2C2CD84_DACD_4550_BEAE_D64EEA903FD9
#define ENTITY_COMPONENTS_INCLUDED_C2C2CD84_DACD_4550_BEAE_D64EEA903FD9


#include <core/common/core_fwd.hpp>


namespace Core {
namespace Entity_utils {


bool
set_camera_component(const Core::Entity_ref &ref, const Core::Camera &camera);


Core::Camera
get_camera_component(const Core::Entity_ref &ref);


bool
has_camera_component(const Core::Entity_ref &ref);


} // ns
} // ns


#endif // inc guard