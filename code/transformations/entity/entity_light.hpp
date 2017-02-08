#ifndef ENTITY_LIGHT_INCLUDED_6120946C_3B4B_4741_9D51_E658D049470F
#define ENTITY_LIGHT_INCLUDED_6120946C_3B4B_4741_9D51_E658D049470F


#include <core/common/core_fwd.hpp>
#include <data/fwd_dec_data.hpp>
#include <data/graph/fwd.hpp>
#include <utilities/generic_id.hpp>


namespace Entity_detail {


void
set_light(const util::generic_id this_id,
          Data::Graph::Graph_data *entity_data,
          Data::Light_data *light_data,
          const Core::Point_light *light);


void
set_light_transform(const util::generic_id this_id,
                    Data::Graph::Graph_data *entity_data,
                    Data::Light_data *light_data);


} // ns


#endif // inc guard