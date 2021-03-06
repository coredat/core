#include <core/model/model.hpp>
#include <core/model/mesh.hpp>
#include <common/error_strings.hpp>
#include <common/fixed_string_search.hpp>
#include <data/world_data.hpp>
#include <data/context_data.hpp>
#include <data/context/mesh_data.hpp>
#include <math/geometry/aabb.hpp>
#include <utilities/file.hpp>
#include <utilities/obj_model_loader.hpp>
#include <utilities/logging.hpp>
#include <utilities/string.hpp>
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
  if(!id)
  {
    return;
  }

  // Check id is valid, if not null it
  {
    auto resources = Data::get_context_data();
    assert(resources);
    
    Data::Mesh_data *mesh_data = resources->mesh_data;
    assert(mesh_data);

    Data::data_lock(mesh_data);
    
    if(!Data::mesh_exists(mesh_data, id))
    {
      LOG_WARNING(Error_string::resource_is_invalid());
      m_impl->mesh_id = 0;
    }

    Data::data_unlock(mesh_data);
  }
}


Model::Model(const char *filename)
: m_impl(new Impl)
{
  const std::string file(filename);
  
  char name[512];
  memset(name, 0, sizeof(name));
  lib::string::filename_from_path(filename, name, 512);
  
  if(!lib::file::exists(filename))
  {
    LOG_ERROR(Error_string::file_not_found());
    return;
  }

  auto resources = Data::get_context_data();
  assert(resources);
  
  Data::Mesh_data *mesh_data = resources->mesh_data;
  assert(mesh_data);

  // Check if id already exists, avoid loading the mesh again.
  {
    m_impl->mesh_id = 0;
  
    Data::data_lock(mesh_data);
    
    size_t search_id = 0;
    
    if(Common::fixed_string_search(name,
                                   Data::mesh_get_name_data(mesh_data),
                                   Data::mesh_get_name_stride(),
                                   Data::mesh_get_size(mesh_data),
                                   &search_id))
    {
      #ifdef LOG_DOUBLE_RESOURCE_INITIALIZE
      LOG_WARNING(Error_string::resource_already_exists());
      #endif
      
      m_impl->mesh_id = mesh_data->keys[search_id];
    }
    
    Data::data_unlock(mesh_data);
    
    if(m_impl->mesh_id)
    {
      return;
    }
  }
  
  const lib::obj_model model(lib::load_obj(file));
  
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
    const lib::gl_mesh imported_mesh = lib::create_open_gl_mesh(pos, tex_c, norm, ind);

    // Create graphics mesh and insert it.
    {
      Graphics_api::Mesh mesh;
      Graphics_api::mesh_create_new(&mesh,
                                    imported_mesh.mesh_data.data(),
                                    imported_mesh.mesh_data.size(),
                                    nullptr,
                                    0);
      
      const math::aabb model_aabb = math::aabb_init_from_xyz_data(model.meshes.at(0).positions.data(), model.meshes.at(0).positions.size());
      
      Data::data_lock(mesh_data);
      
      const uint32_t id = Data::mesh_push(mesh_data);
      Data::mesh_set_mesh(mesh_data, id, &mesh);
      Data::mesh_set_aabb(mesh_data, id, &model_aabb);
      Data::mesh_set_name(mesh_data, id, name, strlen(name));
      
      m_impl->mesh_id = id;

      Data::data_unlock(mesh_data);
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
  
  Data::Mesh_data *mesh_data = Data::get_context_data()->mesh_data;
  assert(mesh_data);
  
  Data::data_lock(mesh_data);
  Data::mesh_get_aabb(mesh_data, m_impl->mesh_id, &return_aabb);
  Data::data_unlock(mesh_data);
    
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
