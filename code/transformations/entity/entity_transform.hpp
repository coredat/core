#ifndef ENTITY_TRANSFORM_INCLUDED_47A6BBBD_501D_4FEC_9A49_1D791C4D6120
#define ENTITY_TRANSFORM_INCLUDED_47A6BBBD_501D_4FEC_9A49_1D791C4D6120


#include <transformations/entity/entity_common.hpp>
#include <data/fwd_dec_data.hpp>
#include <data/graph/fwd.hpp>
#include <data/physics/fwd.hpp>


namespace Entity_detail {


/*
  Sets a transform.
  This needs to change to only deal with transform data, and the calling
  code will need to check if the rigidbody needs updating.
*/
void
set_transform(const uint32_t this_id,
              Data::Graph::Graph_data *entity_data,
              Data::Rigidbody_data *rb_data,
              Data::Physics::Physics_data *phy_world,
              Data::Trigger_data *trigger_data,
              Data::Mesh_draw_call_data *mesh_data,
              Data::Text_draw_call_data *text_data,
              const Core::Transform &transform,
              bool inform_phys_engine = true);


/*
  Gets a math::transform from the entity data.
*/
math::transform
get_transform(const uint32_t this_id,
              Data::Graph::Graph_data *entity_data,
              const Data::Transform_data *transform_data);


/*
  Sugar method, that will convert a math::transform into a Core::Transform.
*/
Core::Transform
get_core_transform(const uint32_t this_id,
                   Data::Graph::Graph_data *entity_data,
                   const Data::Transform_data *transform_data);
  

} // ns


#endif // inc guard
