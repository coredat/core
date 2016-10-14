#ifndef DEBUG_CAPACITIES_INCLUDED_931A61CA_8382_4E91_80F5_FAAE633A3BB7
#define DEBUG_CAPACITIES_INCLUDED_931A61CA_8382_4E91_80F5_FAAE633A3BB7


#include <data/global_data/text_mesh_data.hpp>
#include <data/global_data/material_data.hpp>
#include <data/global_data/mesh_data.hpp>
#include <data/global_data/post_process_data.hpp>
#include <data/global_data/shader_data.hpp>
#include <data/global_data/texture_data.hpp>

#include <data/world_data/camera_data.hpp>
#include <data/world_data/entity_data.hpp>
#include <data/world_data/physics_data.hpp>
#include <data/world_data/renderer_mesh_data.hpp>
#include <data/world_data/renderer_text_draw_calls_data.hpp>
#include <data/world_data/transform.hpp>



namespace Debug_menu {


void
display_capacities(Resource_data::Text_mesh_data *text_mesh_data,
                   Resource_data::Material_data *material_data,
                   Resource_data::Mesh_data *mesh_data,
                   Resource_data::Post_process_data *post_process_data,
                   Resource_data::Shader_data *shader_data,
                   Resource_data::Texture_data *texture_data,
                   
                   World_data::Camera_data *camera_data,
                   World_data::Entity_data *entity_data,
                   World_data::Renderer_mesh_data *renderer_mesh_data,
                   World_data::Renderer_text_draw_calls_data *renderer_text_draw_calls,
                   Data::Transform_data *transforms,
                   World_data::Physics_data *phs_data);


} // ns


#endif // inc guard