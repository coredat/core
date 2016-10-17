#ifndef DEBUG_CAPACITIES_INCLUDED_931A61CA_8382_4E91_80F5_FAAE633A3BB7
#define DEBUG_CAPACITIES_INCLUDED_931A61CA_8382_4E91_80F5_FAAE633A3BB7


#include <data/global_data/text_mesh_data.hpp>
#include <data/global_data/material_data.hpp>
#include <data/global_data/mesh_data.hpp>
#include <data/global_data/post_process_data.hpp>
#include <data/global_data/shader_data.hpp>
#include <data/global_data/texture_data.hpp>

#include <data/world/camera_data.hpp>
#include <data/world/entity_data.hpp>
#include <data/world/rigidbody_data.hpp>
#include <data/world/mesh_draw_call_data.hpp>
#include <data/world/text_draw_call_data.hpp>
#include <data/world/transform_data.hpp>



namespace Debug_menu {


void
display_capacities(Resource_data::Text_mesh_data *text_mesh_data,
                   Resource_data::Material_data *material_data,
                   Resource_data::Mesh_data *mesh_data,
                   Resource_data::Post_process_data *post_process_data,
                   Resource_data::Shader_data *shader_data,
                   Resource_data::Texture_data *texture_data,
                   
                   Data::Camera_data *camera_data,
                   Data::Entity_data *entity_data,
                   Data::Mesh_draw_call_data *renderer_mesh_data,
                   Data::Text_draw_call_data *renderer_text_draw_calls,
                   Data::Transform_data *transforms,
                   Data::Rigidbody_data *rb_data);


} // ns


#endif // inc guard