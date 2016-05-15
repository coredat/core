#ifndef TRANSFORM_DATA_INCLUDED_0D78D4DD_CB3D_4FD3_AF0D_6DD965264754
#define TRANSFORM_DATA_INCLUDED_0D78D4DD_CB3D_4FD3_AF0D_6DD965264754


#include <utilities/generic_id.hpp>
#include <math/transform/transform_types.hpp>
#include <math/geometry/geometry_types.hpp>
#include <stdint.h>
#include <stddef.h>
#include <stdatomic.h>


namespace World_data {


/*!
  Transform data
  --
  Every entity has a transform, we store entities in sequential order.
  A new entity will be pushed to the back. Erasing an entity will
  shuffle down all the entities.
  
  We also keep an unscaled, unmoved aabb for transformations.
*/
struct Transform_data
{
  util::generic_id    *entity_id  = nullptr;
  math::transform     *transform  = nullptr;
  math::aabb          *aabb       = nullptr;
  
  uint32_t            size        = 0;
  const uint32_t      capacity    = 0;
};


void
lock(Transform_data *data);


void
unlock(Transform_data *data);


void
transform_data_init(Transform_data *data,
                    const uint32_t size_hint);


void
transform_data_free(Transform_data *data);


void
transform_data_add_transform(Transform_data *data,
                             const util::generic_id id,
                             math::transform *trans = nullptr,
                             math::aabb *aabb = nullptr);


void
transform_data_remove_transform(Transform_data *data,
                                const util::generic_id id);


bool
transform_data_exists(Transform_data *data,
                      const util::generic_id id,
                      size_t *out_index = nullptr);


} // ns


#endif // inc guard