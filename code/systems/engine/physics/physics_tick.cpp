#include "physics_tick.hpp"

#include <3rdparty/qu3e/q3.h>
#include <transformations/physics/q3_math_extensions.hpp>

#include <3rdparty/qu3e/debug/q3Render.h>
#include <transformations/physics/q3_math_extensions.hpp>
#include <transformations/physics/update_world.hpp>
#include <transformations/entity/entity_transform.hpp>

#include <renderer/debug_line_renderer/debug_line_renderer_node.hpp>
#include <renderer/debug_line_renderer/debug_line_renderer.hpp>

// Don't want core here!
#include <core/common/collision.hpp>
#include <core/transform/transform.hpp>
#include <core/entity/detail/entity_id.hpp>
#include <core/physics/collision.hpp>


namespace {

struct Debug_renderer : public q3Render
{
  math::vec3 pen_pos = math::vec3_zero();
  math::vec3 pen_color = math::vec3_zero();
  math::vec3 scale = math::vec3_one();

  void SetPenColor( f32 r, f32 g, f32 b, f32 a = 1.0f ) override
  {
    pen_color = math::vec3_init(r, g, b);
  }
  
  
	void SetPenPosition( f32 x, f32 y, f32 z ) override
  {
    pen_pos = math::vec3_from_q3vec(q3Vec3(x, y, z));
  }
  
  
	void SetScale( f32 sx, f32 sy, f32 sz ) override
  {
    scale = math::vec3_init(sx, sy, sz);
  }

	// Render a line from pen position to this point.
	// Sets the pen position to the new point.
	void Line( f32 x, f32 y, f32 z ) override
  {
    math::vec3 pos = math::vec3_from_q3vec(q3Vec3(x, y, z));
  
    Debug_line_renderer::Line_node node;
    node.color[0] = math::get_x(pen_color);
    node.color[1] = math::get_y(pen_color);
    node.color[2] = math::get_z(pen_color);
    
    node.position_from[0] = math::get_x(pen_pos);
    node.position_from[1] = math::get_y(pen_pos);
    node.position_from[2] = math::get_z(pen_pos);
    
    node.position_to[0] = math::get_x(pos);
    node.position_to[1] = math::get_y(pos);
    node.position_to[2] = math::get_z(pos);
    
    Debug_line_renderer::add_lines(&node, 1);
  }
  

	void SetTriNormal( f32 x, f32 y, f32 z ) override {}

	// Render a triangle with the normal set by SetTriNormal.
	void Triangle(
		f32 x1, f32 y1, f32 z1,
		f32 x2, f32 y2, f32 z2,
		f32 x3, f32 y3, f32 z3
		) override
  {
    const math::vec3 old_pos = pen_pos;
  
    pen_pos = math::vec3_init(x3, y3, z3);
    Line(x1, y1, z1);
    
    pen_pos = math::vec3_init(x1, y1, z1);
    Line(x2, y2, z2);
    
    pen_pos = math::vec3_init(x2, y2, z2);
    Line(x3, y3, z3);
    
    pen_pos = old_pos;
  }

	// Draw a point with the scale from SetScale
	void Point( ) override {}
} debug_renderer;

  // This state needs to go because its accross worlds
  Core::Collision_callback callback_hack;

} // anon ns


namespace Engine {
namespace Physics_tick {


void
initialize()
{
}


void
collision_callback(Core::Collision_callback callback)
{
  callback_hack = callback;
};


void
think(std::shared_ptr<World_data::World> world, const float dt)
{
  /*
    Collisions
    --
    Don't like the use of Core:: here.
    Where possible we should dump the information into an array, and let core apply
    the transformation to the data to generate Core::Collisions.
    
    Also don't like the callback here. World think should check if any collisions then fire its own callback.
  */
  {
    Core::Collision_pair *collisions_arr = nullptr;
    uint32_t number_of_collisions = 0;
    
    Physics_transform::update_world(world,
                                    &collisions_arr,
                                    &number_of_collisions);
    
    if(number_of_collisions && callback_hack)
    {
      for(uint32_t i = 0; i < number_of_collisions; ++i)
      {
        callback_hack(Core::Collision_type::enter, collisions_arr[i]);
      }
    }
    
    auto to_core_trans = [](const q3Transform &other)
    {
      math::transform trans = math::transform_init_from_q3(other);
      return Core::Transform(trans.position, trans.scale, trans.rotation);
    };
  
    // Set transforms.
    for(size_t i = 0; i < world->physics_data->size; ++i)
    {
      if(world->physics_data->property_rigidbody[i])
      {
        auto trans = reinterpret_cast<q3Body*>(world->physics_data->property_rigidbody[i])->GetTransform();
        
        Core::Entity_ref ref(Core_detail::entity_id_from_uint(world->physics_data->physics_id[i]));
        
        auto old_tran = ref.get_transform();
        
        auto core_trans = to_core_trans(trans);
        core_trans.set_scale(old_tran.get_scale());
        
        Entity_detail::set_transform(world->physics_data->physics_id[i],
                                     world->entity,
                                     world->transform,
                                     world->physics_data,
                                     world->mesh_data,
                                     world->text_data,
                                     core_trans,
                                     false);
      }
    }
  }
}


void
de_initialize()
{
}


} // ns
} // ns