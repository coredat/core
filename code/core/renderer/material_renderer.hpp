#ifndef MATERIAL_RENDERER_INCLUDED_4307B221_E5ED_4329_B7DC_6AE309DCF1AF
#define MATERIAL_RENDERER_INCLUDED_4307B221_E5ED_4329_B7DC_6AE309DCF1AF


#include <core/common/core_fwd.hpp>
#include <core/common/core_types.hpp>


namespace Core {


class Material_renderer final
{
public:

  explicit                Material_renderer(const util::generic_id mat_id,
                                            const util::generic_id model_id); // make private or something like that.
  
  explicit                Material_renderer(const Material &material, const Model &model);
  
  explicit                Material_renderer();
                          ~Material_renderer();
  
  void                    set_material(const Material &material);
  Material                get_material() const;
  
  void                    set_model(const Model &model);
  Model                   get_model() const;
  
                          operator Renderer() const;
  
private:

  friend class Entity;
  friend class Entity_ref;

  util::generic_id        get_material_id() const;
  util::generic_id        get_model_id() const;
  
private:

  util::generic_id m_material_id = util::generic_id_invalid();
  util::generic_id m_model_id    = util::generic_id_invalid();

}; // class


} // ns


#endif // inc guard