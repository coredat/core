#ifndef ENTITY_RIGIDBODY_INCLUDED_B4C31EEC_122B_4023_9E35_436E305B4FE2
#define ENTITY_RIGIDBODY_INCLUDED_B4C31EEC_122B_4023_9E35_436E305B4FE2


#include <transformations/entity/entity_common.hpp>
#include <data/world_data/physics_data.hpp>
#include <math/transform/transform_fwd.hpp>
#include <math/geometry/geometry_fwd.hpp>


namespace Entity_detail {


void
set_collider(const util::generic_id this_id,
             World_data::World *world,
             const Core::Collider &collider);


Core::Collider
get_collider(const util::generic_id this_id,
             World_data::World *world);


void
set_rigidbody(const util::generic_id this_id,
              World_data::World *world,
              const Core::Rigidbody &rigidbody);
  

Core::Rigidbody
get_rigidbody(const util::generic_id this_id);


void
update_collider(const util::generic_id this_id,
                World_data::Entity_data *entity_data,
                World_data::Physics_data *phys_data,
                const math::transform *transform,
                const math::aabb *model_aabb,
                const bool inform_phys_engine = true);


} // ns


#endif // inc guard