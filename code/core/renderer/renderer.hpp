#ifndef RENDERER_INCLUDED_229F0D89_0738_4313_943C_1FB83B3DE604
#define RENDERER_INCLUDED_229F0D89_0738_4313_943C_1FB83B3DE604


#include <core/common/core_types.hpp>
#include <core/common/core_fwd.hpp>
#include <core/renderer/renderer_type.hpp>
#include <core/renderer/renderer_utils.hpp>


/*
  TODO
  --
  - Make main ctor private, want to enforce that this isn't a type you should be using.
  - Add operator cast methods here.
*/


namespace Core {


class Renderer final
{
public:

  explicit                Renderer(const Renderer_type renderer_type,
                                   const util::generic_id arg_01 = util::generic_id_invalid(),
                                   const util::generic_id arg_02 = util::generic_id_invalid(),
                                   const util::generic_id arg_03 = util::generic_id_invalid(),
                                   const util::generic_id arg_04 = util::generic_id_invalid());
  
  Renderer_type           get_type() const;
  const char *            get_type_name() const;
  
private:

  friend Material_renderer Renderer_utils::cast_to_material_renderer(const Renderer&);
  friend Text_renderer     Renderer_utils::cast_to_text_renderer(const Renderer&);
  
  util::generic_id        get_arg_01() const;
  util::generic_id        get_arg_02() const;
  util::generic_id        get_arg_03() const;
  util::generic_id        get_arg_04() const;
  
private:

  const Renderer_type     m_renderer_type = Renderer_type::unknown;
  const util::generic_id  m_arg_01        = util::generic_id_invalid();
  const util::generic_id  m_arg_02        = util::generic_id_invalid();
  const util::generic_id  m_arg_03        = util::generic_id_invalid();
  const util::generic_id  m_arg_04        = util::generic_id_invalid();

};


} // ns


#endif // inc guard