#include "model.hpp"
#include <data/world_data/world.hpp>
#include <data/world_data/model_pool.hpp>


namespace Core {


Model::Model(const uint32_t model_resource_id)
: m_model_id(model_resource_id)
{
}


uint32_t
Model::get_number_of_meshes() const
{
  return 0;
}


Mesh
Model::get_mesh(const uint32_t i) const
{
  World_data::World *world = World_data::get_world();
  
  if(world)
  {
    //world_data->model_pool;
  }

  return Mesh();
}


bool
Model::set_mesh(const uint32_t i, const Mesh &mesh)
{
  return false;
}


} // ns