#ifndef ENTITY_TRANSFORM_INCLUDED_47A6BBBD_501D_4FEC_9A49_1D791C4D6120
#define ENTITY_TRANSFORM_INCLUDED_47A6BBBD_501D_4FEC_9A49_1D791C4D6120


#include <transformations/entity/entity_common.hpp>


namespace Entity_detail {


void
set_transform(const util::generic_id this_id,
              World_data::World *world,
              const Core::Transform &transform,
              bool inform_phys_engine = true);


Core::Transform
get_transform(const util::generic_id this_id,
              World_data::World *world);


} // ns


#endif // inc guard