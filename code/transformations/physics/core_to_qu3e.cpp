#include <transformations/physics/core_to_qu3e.hpp>
#include <core/physics/rigidbody.hpp>
#include <core/physics/box_collider.hpp>
#include <core/physics/collider.hpp>
#include <core/physics/box_collider_utils.hpp>
#include <core/physics/collider_utils.hpp>
#include <core/transform/transform.hpp>
#include <data/global_data/memory_data.hpp>


namespace Physics_transform {


void
convert_core_rb_to_qu3e(const Core::Rigidbody core_rb[],
                        const Core::Transform core_transform[],
                        q3Body *out_rbs[],
                        q3Scene *scene,
                        const uint32_t rb_count)
{
  /*
    Need to benchmark this at some point,
    niggling feeling this can be vastly improved.
  */
  
  // Build the body defs
  q3BodyDef *body_def = SCRATCH_ALIGNED_ALLOC(q3BodyDef, rb_count);

  for(uint32_t i = 0; i < rb_count; ++i)
  {
    const Core::Rigidbody &rigidbody = core_rb[i];
    const Core::Transform &transform = core_transform[i];
  
    body_def[i].allowSleep = false;
    body_def[i].position.Set(math::get_x(transform.get_position()),
                             math::get_y(transform.get_position()),
                             math::get_z(transform.get_position()));
        
    if(rigidbody.is_dynamic())
    {
      body_def[i].bodyType = eDynamicBody;
    }
    else
    {
      body_def[i].bodyType = eStaticBody;
    }
  }
  
  // Build the box defs
  q3BoxDef *box_def = SCRATCH_ALIGNED_ALLOC(q3BoxDef, rb_count);
  
  for(uint32_t i = 0; i < rb_count; ++i)
  {
    const Core::Rigidbody &rigidbody = core_rb[i];
    const Core::Transform &transform = core_transform[i];
    
    q3Transform localSpace;
    q3Identity(localSpace);
    
    box_def[i].Set(localSpace, q3Vec3(math::get_x(transform.get_scale()),
                                      math::get_y(transform.get_scale()),
                                      math::get_z(transform.get_scale())));
    
    // Mass = density * volume
    // Density = Mass / volume
    {
      if(rigidbody.get_mass())
      {
        const float volume = Core::Box_collider_utils::get_volume(Core::Collider_utis::cast_to_box_collider(rigidbody.get_collider()));

        const float density = rigidbody.get_mass() / volume;
        box_def[i].SetDensity(density);
      }
      else
      {
        box_def[i].SetDensity(0);
        box_def[i].SetRestitution(1.f);
      }
    }
  }

  // Generate bodies
  for(uint32_t i = 0; i < rb_count; ++i)
  {
    out_rbs[i] = scene->CreateBody(body_def[i]);
    out_rbs[i]->AddBox(box_def[i]);
  }
}


} // ns