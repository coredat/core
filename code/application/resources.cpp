#include "resources.hpp"
#include <systems/model_manager/model_manager.hpp>
#include <systems/texture_manager/texture_manager.hpp>
#include <systems/entity/generic_id.hpp>
#include <data/texture_pool.hpp>
#include <data/model_pool.hpp>
#include <string>
#include <utils/directory.hpp>


namespace Resource {


void
load_default_resources(Data::Texture_pool *texture_pool,
                       std::size_t size_of_texture_pool,
                       Data::Model_pool *model_pool,
                       std::size_t size_of_model_pool)
{
  const std::string asset_path = util::get_resource_path() + "assets/";

  // Load models
  {
    const ::Model::Load_model model[Resource::Model::size] {
      ::Model::Load_model{Resource::Model::unit_cube,   std::string(asset_path + "models/unit_cube.obj")},
      ::Model::Load_model{Resource::Model::unit_plane,  std::string(asset_path + "models/unit_plane.obj")},
    };
    
    Generic_id::ids_init_sequentially(model_pool->id, model_pool->size);
    ::Model::model_load(model, Resource::Model::size, model_pool->vbo, model_pool->size);
  }
  
  // Load textures
  {
    const ::Texture::Load_texture tex[Resource::Texture::size] {
      ::Texture::Load_texture{Resource::Texture::dev_green,             std::string(asset_path + "textures/dev_grid_green_512.png").c_str()},
      ::Texture::Load_texture{Resource::Texture::dev_red,               std::string(asset_path + "textures/dev_grid_red_512.png").c_str()},
      ::Texture::Load_texture{Resource::Texture::dev_blue,              std::string(asset_path + "textures/dev_grid_blue_512.png").c_str()},
      ::Texture::Load_texture{Resource::Texture::dev_orange,            std::string(asset_path + "textures/dev_grid_orange_512.png").c_str()},
      ::Texture::Load_texture{Resource::Texture::dev_colored_squares,   std::string(asset_path + "textures/dev_colored_squares_512.png").c_str()},
      ::Texture::Load_texture{Resource::Texture::dev_squares,           std::string(asset_path + "textures/dev_squares_512.png").c_str()},
    };
    
    Generic_id::ids_init_sequentially(texture_pool->id, texture_pool->size);
    ::Texture::texture_load(tex, Resource::Texture::size, texture_pool->texture, texture_pool->size);
  }
}


} // ns