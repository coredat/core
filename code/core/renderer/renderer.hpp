#ifndef RENDERER_INCLUDED_229F0D89_0738_4313_943C_1FB83B3DE604
#define RENDERER_INCLUDED_229F0D89_0738_4313_943C_1FB83B3DE604


#include <core/common/core_types.hpp>
#include <core/common/core_fwd.hpp>
#include <core/renderer/renderer_type.hpp>
#include <core/renderer/detail/renderer_detail_fwd.hpp>
#include <core/entity/detail/entity_detail_fwd.hpp>


/*
  TODO
  --
  - Do i need the cctor asignment etc.
*/


namespace Core {


/*
  This is an interchange class.
  You should use the specific renderer types.
*/
class Renderer final
{
  friend class Material_renderer;
  friend class Text_renderer;
  friend Core::Renderer Entity_detail::get_renderer(const util::generic_id, Data::Entity_data *,Data::Mesh_draw_call_data *);
  
  explicit                  Renderer(const uint32_t renderer_type,
                                     const util::generic_id arg_01 = util::generic_id_invalid(),
                                     const util::generic_id arg_02 = util::generic_id_invalid(),
                                     const util::generic_id arg_03 = util::generic_id_invalid());
public:

  explicit                  Renderer();

  Renderer&                 operator=(const Renderer &other);
  
  uint32_t                  get_type() const;
  const char *              get_type_name() const; // free function
  
                            operator bool() const;
                            operator Material_renderer() const;
                            operator Text_renderer() const;
  
private:

  friend class Entity;
  friend class Entity_ref;

  friend Material_renderer  Renderer_utils::cast_to_material_renderer(const Renderer&);
  friend Text_renderer      Renderer_utils::cast_to_text_renderer(const Renderer&);
  
  util::generic_id          get_arg_01() const;
  util::generic_id          get_arg_02() const;
  util::generic_id          get_arg_03() const;
  
private:

  uint32_t          m_renderer_type = Renderer_type::unknown;
  util::generic_id  m_arg_01        = util::generic_id_invalid();
  util::generic_id  m_arg_02        = util::generic_id_invalid();
  util::generic_id  m_arg_03        = util::generic_id_invalid();

};


} // ns


#endif // inc guard