#ifndef ENTITY_CAMERA_INCLUDED_CFCEEF1D_BEA3_4899_ABB6_6AA2F57945B5
#define ENTITY_CAMERA_INCLUDED_CFCEEF1D_BEA3_4899_ABB6_6AA2F57945B5


#include <core/common/core_fwd.hpp>
#include <data/fwd_dec_data.hpp>
#include <utilities/generic_id.hpp>


namespace Entity_detail {


void
set_camera(const util::generic_id this_id,
           Data::Entity_data *entity_data,
           Data::Camera_data *camera_data,
           const Core::Camera *camera);


void
get_camera(const util::generic_id this_id,
           const Data::Camera_data *camera_data,
           Core::Camera *out_camera);


void
remove_camera(const util::generic_id this_id,
              Data::Entity_data *entity_data,
              Data::Camera_data *camera_data);
  

bool
has_camera(const util::generic_id this_id,
           Data::Camera_data *camera_data);


} // ns


#endif // inc guard