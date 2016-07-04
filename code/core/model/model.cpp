#include <core/model/model.hpp>
#include <core/model/mesh.hpp>
#include <common/error_strings.hpp>
#include <data/world_data/world.hpp>
#include <data/world_data/world_pools.hpp>
#include <data/global_data/resource_data.hpp>
#include <math/geometry/aabb.hpp>
#include <utilities/file_helpers.hpp>
#include <utilities/obj_model_loader.hpp>
#include <utilities/logging.hpp>
#include <utilities/string_helpers.hpp>
#include <graphics_api/mesh.hpp>


namespace Core {


struct Model::Impl
{
  uint32_t mesh_id;
};


Model::Model()
: Model((uint32_t)0)
{
}


Model::Model(const uint32_t id)
: m_impl(new Impl{id})
{
  LOG_TODO("Check this id is valid");
}


Model::Model(const char *filename)
: m_impl(new Impl)
{
  const std::string file(filename);
  const std::string name(util::get_filename_from_path(file));
  
  if(!util::file::exists(filename))
  {
    LOG_ERROR(Error_string::file_not_found());
    return;
  }

  Resource_data::Resources *resources = Resource_data::get_resources();
  assert(resources);
  
  Resource_data::Mesh_data *mesh_data = resources->mesh_data;
  assert(mesh_data);

  // Check if id already exists, avoid loading the mesh again.
  {
    Resource_data::data_lock(mesh_data);
    
    util::generic_id search_id = util::generic_id_invalid();
  
    if(Resource_data::mesh_data_search_property_name(mesh_data, name.c_str(), &search_id))
    {
      #ifdef LOG_DOUBLE_RESOURCE_INITIALIZE
      LOG_WARNING(Error_string::resource_already_exists());
      #endif
    
      m_impl->mesh_id = search_id;
    }
    
    Resource_data::data_unlock(mesh_data);
    
    if(search_id)
    {
      return;
    }
  }
  
  const util::obj_model model(util::load_obj(file));
  
  // No data.
  if(model.meshes.empty())
  {
    assert(false);
    return;
  }

  // Add mesh to mesh manager.
  if(!model.meshes.empty())
  {
    std::vector<float> pos(std::begin(model.meshes.at(0).positions), std::end(model.meshes.at(0).positions));
    std::vector<float> norm(std::begin(model.meshes.at(0).normals), std::end(model.meshes.at(0).normals));
    std::vector<float> tex_c(std::begin(model.meshes.at(0).uvs), std::end(model.meshes.at(0).uvs));
    std::vector<uint32_t> ind(std::begin(model.meshes.at(0).index), std::end(model.meshes.at(0).index));
    const util::gl_mesh imported_mesh = util::create_open_gl_mesh(pos, tex_c, norm, ind);

    // Create graphics mesh and insert it.
    {
      Graphics_api::Mesh mesh;
      Graphics_api::mesh_create_new(&mesh,
                                    imported_mesh.mesh_data.data(),
                                    imported_mesh.mesh_data.size(),
                                    nullptr,
                                    0);
      
      const math::aabb model_aabb = math::aabb_init_from_xyz_data(model.meshes.at(0).positions.data(), model.meshes.at(0).positions.size());
      
      Resource_data::data_lock(mesh_data);
      
      Resource_data::mesh_data_push_back(mesh_data, mesh_data->size + 1);
      Resource_data::mesh_data_set_property_mesh(mesh_data, mesh_data->size, mesh);
      Resource_data::mesh_data_set_property_aabb(mesh_data, mesh_data->size, model_aabb);
      Resource_data::mesh_data_set_property_name(mesh_data, mesh_data->size, name.c_str());
      
      m_impl->mesh_id = mesh_data->size;

      Resource_data::data_unlock(mesh_data);
    }
    
  }
}



Model::~Model()
{
}


Model::Model(const Model &other)
: m_impl(new Impl(*other.m_impl))
{
}


Model::Model(Model &&other)
: m_impl(new Impl(*other.m_impl))
{
}


Model&
Model::operator=(const Model &other)
{
  m_impl.reset(new Impl);
  m_impl->mesh_id = other.m_impl->mesh_id;
  return *this;
}


Model&
Model::operator=(Model &&other)
{
  m_impl.reset(new Impl);
  m_impl->mesh_id = other.m_impl->mesh_id;
  return *this;
}


bool
Model::has_meshes() const
{
  if(m_impl && m_impl->mesh_id)
  {
    return true;
  }
  
  return false;
}


Mesh
Model::get_mesh(const uint32_t index) const
{
  if(m_impl && m_impl->mesh_id)
  {
    // Get mesh
  }
  
  return Mesh();
}


math::aabb
Model::get_model_aabb() const
{
  // If no aabb
  if(!m_impl || !m_impl->mesh_id)
  {
    const math::aabb zero_aabb = math::aabb_init(math::vec3_zero(), math::vec3_zero());
    return zero_aabb;
  }

  math::aabb return_aabb;
  
  Resource_data::Mesh_data *mesh_data = Resource_data::get_resources()->mesh_data;
  assert(mesh_data);
  
  Resource_data::data_lock(mesh_data);
  Resource_data::mesh_data_get_property_aabb(mesh_data, m_impl->mesh_id, &return_aabb);
  Resource_data::data_unlock(mesh_data);
    
  return return_aabb;
}


uint32_t
Model::get_id() const
{
  if(m_impl)
  {
    return m_impl->mesh_id;
  }
  
  return 0;
}


bool
Model::exists() const
{
  return !!m_impl->mesh_id;
}


Model::operator bool() const
{
  return exists();
}

} // ns