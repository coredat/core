#ifndef ENTITY_LIGHT_INCLUDED_6120946C_3B4B_4741_9D51_E658D049470F
#define ENTITY_LIGHT_INCLUDED_6120946C_3B4B_4741_9D51_E658D049470F


#include <core/common/core_fwd.hpp>
#include <data/fwd_dec_data.hpp>
#include <data/graph/fwd.hpp>


namespace Entity_detail {


void
set_light(const uint32_t this_id,
          Data::Graph::Graph_data *entity_data,
          Data::Light_data *light_data,
          const Core::Point_light *light);


void
set_light_transform(const uint32_t this_id,
                    Data::Graph::Graph_data *entity_data,
                    Data::Light_data *light_data);


} // ns


#endif // inc guard
