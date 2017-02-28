//#ifndef ENTITY_RIGIDBODY_INCLUDED_B4C31EEC_122B_4023_9E35_436E305B4FE2
//#define ENTITY_RIGIDBODY_INCLUDED_B4C31EEC_122B_4023_9E35_436E305B4FE2
//
//
//#include <transformations/entity/entity_common.hpp>
//#include <data/fwd_dec_data.hpp>
//#include <data/graph/fwd.hpp>
//#include <data/physics/fwd.hpp>
//
//
//namespace Entity_detail {
//
//
//void
//set_phy_transform(const uint32_t this_id,
//                  const Core::Transform *transform,
//                  Data::Graph::Graph_data *entity_data,
//                  Data::Rigidbody_data *rb_data,
//                  Data::Physics::Physics_data *phy_world,
//                  Data::Trigger_data *trigger_data);
//
//
//void
//set_rigidbody(const uint32_t this_id,
//              Core::Transform tranform,
//              const math::aabb &aabb,
//              const Core::Rigidbody &rigidbody,
//              Data::Graph::Graph_data *entity_data,
//              Data::Trigger_data *trigger_data,
//              Data::Rigidbody_data *rigidbody_data,
//              Data::Physics::Physics_data *physics_world);
//  
//
//Core::Rigidbody
//get_rigidbody(const uint32_t this_id,
//              const Data::Graph::Graph_data *entity_data,
//              const Data::Rigidbody_data *rb_data,
//              const Data::Trigger_data *trigger_data);
//  
//
//void
//apply_force(const uint32_t this_id,
//            const Data::Rigidbody_data *rb_data,
//            const math::vec3 direction,
//            const float power);
//
//
//} // ns
//
//
//#endif // inc guard
