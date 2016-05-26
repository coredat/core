#include <core/renderer/material_renderer.hpp>
#include <core/entity/entity.hpp>
#include <core/entity/entity_ref.hpp>
#include <core/color/color.hpp>
#include <core/color/color_utils.hpp>
#include <core/camera/camera_properties.hpp>
#include <core/transform/transform.hpp>

#include <graphics_api/clear.hpp>
#include <graphics_api/initialize.hpp>
#include <graphics_api/mesh.hpp>

#include <renderer/simple_renderer/simple_renderer_node.hpp>
#include <renderer/simple_renderer/simple_renderer.hpp>
#include <renderer/debug_line_renderer/debug_line_renderer_node.hpp>
#include <renderer/debug_line_renderer/debug_line_renderer.hpp>
#include <renderer/renderer.hpp>
#include <renderer/simple_renderer/simple_renderer.hpp>

#include <data/global_data/resource_data.hpp>

#include <systems/transform/transformations.hpp>
#include <data/world_data/world_data.hpp>
#include <math/math.hpp>
#include <vector>



namespace Core {


struct Material_renderer::Impl
{
  
};


Material_renderer::Material_renderer()
: m_impl(new Impl)
{
}


Material_renderer::~Material_renderer()
{
}


} // ns