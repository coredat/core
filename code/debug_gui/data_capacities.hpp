#ifndef DEBUG_CAPACITIES_INCLUDED_931A61CA_8382_4E91_80F5_FAAE633A3BB7
#define DEBUG_CAPACITIES_INCLUDED_931A61CA_8382_4E91_80F5_FAAE633A3BB7


#include <data/fwd_dec_data.hpp>


namespace Debug_menu {


void
display_capacities(Data::Text_mesh_data *text_mesh_data,
                   Data::Material_data *material_data,
                   Data::Mesh_data *mesh_data,
                   Data::Post_process_data *post_process_data,
                   Data::Shader_data *shader_data,
                   Data::Texture_data *texture_data,
                   
                   Data::Camera_data *camera_data,
                   Data::Entity_data *entity_data,
                   Data::Mesh_draw_call_data *renderer_mesh_data,
                   Data::Text_draw_call_data *renderer_text_draw_calls,
                   Data::Transform_data *transforms,
                   Data::Rigidbody_data *rb_data);


} // ns


#endif // inc guard