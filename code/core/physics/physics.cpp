#include <core/physics/physics.hpp>
#include <core/entity/entity_ref.hpp>
#include <core/entity/detail/entity_id.hpp>
#include <core/world/detail/world_index.hpp>
#include <transformations/entity/entity_rigidbody.hpp>
#include <data/physics/physics.hpp>


namespace Core {
namespace Physics {


void
apply_force(
  Core::Entity_ref ref,
  const math::vec3 world_direction,
  const float force)
{
  const Core_detail::Entity_id entity_id(
    Core_detail::entity_id_from_uint(ref.get_id())
  );
  
  auto world_data = Core_detail::world_index_get_world_data(
    entity_id.world_instance
  );
  
  const Data::Physics::Physics_data *phys = world_data->physics;

  Data::Physics::rigidbody_apply_force(
    phys,
    ref.get_id(),
    world_direction,
    force
  );
}


} // ns
} // ns
