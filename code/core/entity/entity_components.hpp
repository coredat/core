#ifndef ENTITY_COMPONENTS_INCLUDED_C2C2CD84_DACD_4550_BEAE_D64EEA903FD9
#define ENTITY_COMPONENTS_INCLUDED_C2C2CD84_DACD_4550_BEAE_D64EEA903FD9


#include <core/common/core_fwd.hpp>


namespace Core {
namespace Entity_component {


bool
set_camera(const Core::Entity_ref &ref,
           const Core::Camera &camera);


Core::Camera
get_camera(const Core::Entity_ref &ref);


void
remove_camera(const Core::Entity_ref &ref);


bool
has_camera(const Core::Entity_ref &ref);


} // ns
} // ns


#endif // inc guard