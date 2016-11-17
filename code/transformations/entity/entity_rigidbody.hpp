#ifndef ENTITY_RIGIDBODY_INCLUDED_B4C31EEC_122B_4023_9E35_436E305B4FE2
#define ENTITY_RIGIDBODY_INCLUDED_B4C31EEC_122B_4023_9E35_436E305B4FE2


#include <transformations/entity/entity_common.hpp>
#include <transformations/physics/bullet/bullet_fwd.hpp>
#include <data/fwd_dec_data.hpp>


namespace Entity_detail {


void
set_phy_transform(const util::generic_id this_id,
                  const Core::Transform *transform,
                  Data::Entity_data *entity_data,
                  Data::Rigidbody_data *rb_data,
                  Bullet_data::World *phy_world,
                  Data::Trigger_data *trigger_data);


void
set_rigidbody(const util::generic_id this_id,
              const Core::Rigidbody &rigidbody,
              const Data::Transform_data *transform_data,
              Data::Entity_data *entity_data,
              Data::Trigger_data *trigger_data,
              Data::Rigidbody_data *rigidbody_data,
              Bullet_data::World *physics_world);
  

Core::Rigidbody
get_rigidbody(const util::generic_id this_id,
              const Data::Entity_data *entity,
              const Data::Transform_data *transforms,
              const Data::Rigidbody_data *rb_data,
              const Data::Trigger_data *trigger_data);
  

void
apply_force(const util::generic_id this_id,
            const Data::Rigidbody_data *rb_data,
            const math::vec3 direction,
            const float power);


} // ns


#endif // inc guard