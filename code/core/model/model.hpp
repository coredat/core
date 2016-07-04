#ifndef MODEL_INCLUDED_292C333A_8EB1_40B3_A4AA_19E7E9D0447A
#define MODEL_INCLUDED_292C333A_8EB1_40B3_A4AA_19E7E9D0447A


#include <core/common/core_fwd.hpp>
#include <core/common/core_types.hpp>


namespace Core {


class Model final
{
public:

  explicit            Model();
  explicit            Model(const uint32_t id);
  explicit            Model(const char *filename);
                      ~Model();
  
                      Model(const Model&);
                      Model(Model&&);
  
  Model&              operator=(const Model&);
  Model&              operator=(Model&&);

  bool                has_meshes() const;
  Mesh                get_mesh(const uint32_t index) const;
  uint32_t            get_number_of_meshes() const;
  
  math::aabb          get_model_aabb() const;
  
  uint32_t            get_id() const;
  
  bool                exists() const;
                      operator bool() const; // Alias for exists().
  
private:

  struct Impl;
  std::unique_ptr<Impl> m_impl;

}; // class


} // ns


#endif // inc guard