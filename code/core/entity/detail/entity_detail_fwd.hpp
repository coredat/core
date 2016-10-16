#ifndef ENTITY_DETAIL_FWD_INCLUDED_2627D343_A182_407C_A9CE_4A4521CD49A9
#define ENTITY_DETAIL_FWD_INCLUDED_2627D343_A182_407C_A9CE_4A4521CD49A9


#include <core/renderer/renderer_fwd.hpp>
#include <data/world_data/world_data_fwd.hpp>


namespace Core_detail {


struct Entity_id;


} // ns


namespace Entity_detail {


// Prototype the function here so that we can use it as a friend dec.
[[deprecated("These little friend things need to go")]]
Core::Renderer
get_renderer(const util::generic_id this_id,
             Data::Entity_data *entity_data,
             World_data::Renderer_mesh_data *renderer_material);


} // ns


#endif // inc guard