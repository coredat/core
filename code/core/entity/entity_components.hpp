#ifndef ENTITY_COMPONENTS_INCLUDED_C2C2CD84_DACD_4550_BEAE_D64EEA903FD9
#define ENTITY_COMPONENTS_INCLUDED_C2C2CD84_DACD_4550_BEAE_D64EEA903FD9


#include <core/common/core_fwd.hpp>


namespace Core {
namespace Entity_component {


/*
  Transform Component
  --
  Represents an entity's position in the the world.
  You cannot remove a transform from an entity.
*/

bool
set_transform(const Core::Entity_ref &ref,
             const Core::Transform &transform);


Core::Transform
get_transform(const Core::Entity_ref &ref);


bool
has_transform(const Core::Entity_ref &ref);


/*
  Camera Component
  --
  An attached camera will use the transform of the entity
  to render the world.
*/

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