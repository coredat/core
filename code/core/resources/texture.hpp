#ifndef TEXTURE_INCLUDED_4BAC239C_846B_4C57_9B2D_F49FB4E96062
#define TEXTURE_INCLUDED_4BAC239C_846B_4C57_9B2D_F49FB4E96062


#include <core/common/core_types.hpp>


namespace Core {


class Texture final
{
public:

  explicit            Texture();
  explicit            Texture(const util::generic_id id);
  explicit            Texture(const char * filepath);
  
                      ~Texture();
  
                      Texture(const Texture&);
                      Texture(Texture&&);
  
  Texture&            operator=(const Texture&);
  Texture&            operator=(Texture&&);
  
  uint32_t            get_width() const;
  uint32_t            get_height() const;
  
  bool                exists() const;
                      operator bool() const; // Alias for exists().
  
  util::generic_id    get_id() const;
  
private:

  struct Impl;
  std::unique_ptr<Impl> m_impl;

}; // class


} // ns


#endif // inc guard