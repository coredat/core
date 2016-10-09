#include <transformations/physics/core_to_qu3e.hpp>
#include <core/physics/rigidbody.hpp>
#include <core/physics/box_collider.hpp>
#include <core/physics/collider.hpp>
#include <core/physics/box_collider_utils.hpp>
#include <core/physics/collider_utils.hpp>
#include <core/transform/transform.hpp>
#include <data/global_data/memory_data.hpp>
#include <utilities/logging.hpp>


namespace Physics_transform {


void
convert_core_rb_to_qu3e(const util::generic_id user_data[],
                        const Core::Rigidbody core_rb[],
                        const Core::Transform core_transform[],
                        q3Body *out_rbs[],
                        q3Scene *scene,
                        const uint32_t rb_count)
{
  /*
    Need to benchmark this at some point,
    niggling feeling this can be vastly improved.
  */
  LOG_TODO_ONCE("benchmark this.");
  
  // Build the body defs
  q3BodyDef *body_def = SCRATCH_ALIGNED_ALLOC(q3BodyDef, rb_count);

  for(uint32_t i = 0; i < rb_count; ++i)
  {
    const Core::Rigidbody &rigidbody = core_rb[i];
    const Core::Transform &transform = core_transform[i];
    
    body_def[i] = q3BodyDef();
    
    body_def[i].allowSleep = false;
    body_def[i].position.Set(math::get_x(transform.get_position()),
                             math::get_y(transform.get_position()),
                             math::get_z(transform.get_position()));
    
    // Testing this isn't angle axis.
    body_def[i].angle = math::quat_get_w(transform.get_rotation());
    body_def[i].axis = q3Vec3(math::quat_get_x(transform.get_rotation()), math::quat_get_y(transform.get_rotation()), math::quat_get_z(transform.get_rotation()));
    
    if(rigidbody.is_dynamic())
    {
      body_def[i].bodyType = eDynamicBody;
    }
    else
    {
      body_def[i].bodyType = eStaticBody;
    }
    
    body_def[i].userData = util::generic_id_to_ptr(user_data[i]);
  }
  
  // Build the box defs
  q3BoxDef *box_def = SCRATCH_ALIGNED_ALLOC(q3BoxDef, rb_count);
  
  for(uint32_t i = 0; i < rb_count; ++i)
  {
    const Core::Rigidbody &rigidbody = core_rb[i];
    const Core::Transform &transform = core_transform[i];
    
    box_def[i] = q3BoxDef();
    
    q3Transform local_space;
    q3Identity(local_space);

    const Core::Box_collider box_collider = Core::Collider_utis::cast_to_box_collider(rigidbody.get_collider());
    const math::vec3 collider_scale = math::vec3_init(box_collider.get_x_half_extent() * 2.f, box_collider.get_y_half_extent() * 2.f, box_collider.get_z_half_extent() * 2.f);

    box_def[i].Set(local_space, q3Vec3(math::get_x(transform.get_scale()) * math::get_x(collider_scale),
                                       math::get_y(transform.get_scale()) * math::get_y(collider_scale),
                                       math::get_z(transform.get_scale()) * math::get_z(collider_scale)));
    
    // Set trigger or not
    {
      if(!rigidbody.is_trigger())
      {
        if(rigidbody.get_mass())
        {
          // Calculate the density.
          // Mass = density * volume
          // Density = Mass / volume
          const float volume = Core::Box_collider_utils::get_volume(Core::Collider_utis::cast_to_box_collider(rigidbody.get_collider()));

          const float density = rigidbody.get_mass() / volume;
          box_def[i].SetDensity(density);
        }
        else
        {
          box_def[i].SetRestitution(0.f);
        }
      }
      else
      {
        box_def[i].SetSensor(true);
//        box_def[i].SetDensity(0.f);
//        box_def[i].SetRestitution(0.f);
      }
    }
  }

  // Generate bodies
  for(uint32_t i = 0; i < rb_count; ++i)
  {
    q3Body *body = scene->CreateBody(body_def[i]);
    body->AddBox(box_def[i]);
    
    out_rbs[i] = body;
  }
}


} // ns