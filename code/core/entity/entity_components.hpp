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
  Renderer Component
  --
  Attaching an entity to the renderer will use
  the entities transform to render the object.
*/

bool
set_renderer(const Core::Entity_ref &ref,
             const Core::Renderer &renderer);
  

Core::Renderer
get_renderer(const Core::Entity_ref &ref);


void
remove_renderer(const Core::Entity_ref &renderer);


bool
has_renderer(const Core::Entity_ref &ref);


/*
  Rigidbody Component
  --
  Attaching a rigidbody will use the use physics
  engine to update the entities transform.
*/

bool
set_rigidbody(const Core::Entity_ref &ref,
              const Core::Rigidbody &renderer);
  

Core::Rigidbody
get_rigidbody(const Core::Entity_ref &ref);


void
remove_rigidbody(const Core::Entity_ref &renderer);


bool
has_rigidbody(const Core::Entity_ref &ref);


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