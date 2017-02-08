#include <transformations/physics/bullet/motion_state.hpp>
#include <transformations/physics/bullet/bullet_math_extensions.hpp>
#include <transformations/entity/entity_transform.hpp>
#include <core/entity/detail/entity_id.hpp>
#include <core/world/detail/world_index.hpp>
#include <core/transform/transform.hpp>
#include <data/graph/graph.hpp>


namespace Physics_transform {


Core_motion_state::Core_motion_state(const uintptr_t user_data,
                  const btTransform& startTrans,
                  const btTransform& centerOfMassOffset)
: btDefaultMotionState(startTrans, centerOfMassOffset)
{
  m_userPointer = (void*)user_data;
}


void
Core_motion_state::getWorldTransform(btTransform& centerOfMassWorldTrans) const
{
  btDefaultMotionState::getWorldTransform(centerOfMassWorldTrans);
}


void
Core_motion_state::setWorldTransform(const btTransform& centerOfMassWorldTrans)
{
  btDefaultMotionState::setWorldTransform(centerOfMassWorldTrans);
  
  /*
    We need to inform our data that the transform has changed.
    this saves us looping through all the entities.
    --
    However a btTransform doesn't really have the concept of scale
    so we need to preserve the scale from the existing tranform,
    other wise rendered objects will have the wrong scale.
  */
  
  const Core_detail::Entity_id id               = Core_detail::entity_id_from_uint((uintptr_t)m_userPointer);
  const uint32_t               this_id          = Core_detail::entity_id_to_uint(id);
  const Core::Transform        update_transform = math::transform_from_bt(centerOfMassWorldTrans);
  
  auto world_data = Core_detail::world_index_get_world_data(id.world_instance);
  
  // Need to get the transform to preserve the scale.
  math::transform curr_transform;
  Data::Graph::transform_get(world_data->scene_graph, this_id, &curr_transform);
  
  const Core::Transform new_transform(
    update_transform.get_position(),
    curr_transform.scale,
    update_transform.get_rotation()
  );
  
  Entity_detail::set_transform((uintptr_t)m_userPointer,
                               world_data->scene_graph,
                               nullptr,
                               nullptr,
                               nullptr,
                               world_data->mesh_data,
                               world_data->text_data,
                               new_transform,
                               false);

//  
}


} // ns