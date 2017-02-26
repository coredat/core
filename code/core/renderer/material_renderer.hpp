#ifndef MATERIAL_RENDERER_INCLUDED_4307B221_E5ED_4329_B7DC_6AE309DCF1AF
#define MATERIAL_RENDERER_INCLUDED_4307B221_E5ED_4329_B7DC_6AE309DCF1AF


#include <core/common/core_fwd.hpp>
#include <core/common/core_types.hpp>
#include <core/renderer/renderer.hpp>


namespace Core {


class Material_renderer final
{
public:

  explicit                Material_renderer(const uint32_t mat_id,
                                            const uint32_t model_id); // make private or something like that.
  
  explicit                Material_renderer(const Material &material,
                                            const Model &model);
  
  explicit                Material_renderer();
                          ~Material_renderer();
  
  void                    set_material(const Material &material);
  Material                get_material() const;
  
  void                    set_model(const Model &model);
  Model                   get_model() const;
  
  bool                    is_valid() const;
  
                          operator bool() const;
  
//private:

  friend class Entity;
  friend class Entity_ref;

  uint32_t        get_material_id() const;
  uint32_t        get_model_id() const;
  
private:

  uint32_t m_material_id = 0;
  uint32_t m_model_id    = 0;

}; // class


} // ns


#endif // inc guard
