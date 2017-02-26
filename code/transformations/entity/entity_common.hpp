#ifndef ENTITY_COMMON_INCLUDED_861E3180_911C_4588_AA68_98BD4F51CD3E
#define ENTITY_COMMON_INCLUDED_861E3180_911C_4588_AA68_98BD4F51CD3E


#include <core/common/core_fwd.hpp>
#include <data/fwd_dec_data.hpp>
#include <data/graph/fwd.hpp>
#include <stdint.h>


namespace Entity_detail {


uint32_t
get_id(const uint32_t this_id,
       Data::Graph::Graph_data *entity_data);


bool
is_valid(const uint32_t this_id,
         Data::Graph::Graph_data *entity_data,
         const bool emit_error = false);


void
destroy(const uint32_t this_id,
        Data::Graph::Graph_data *entity_data,
        Data::Pending_entity_removal_data *scene_graph_changes);


void
update_component(const uint32_t this_id,
                 Data::Graph::Graph_data *entity_data,
                 const uint32_t component_id);


} // ns


#endif // inc guard
