#ifndef SONG_INCLUDED_E3264BBC_680C_43FC_A322_939E18A1F07A
#define SONG_INCLUDED_E3264BBC_680C_43FC_A322_939E18A1F07A


#include <core/common/core_types.hpp>


namespace Core {


class Song final
{
public:

  explicit        Song();
  
  

private:

  struct Impl;
  std::unique_ptr<Impl> m_impl;

}; // class


} // ns


#endif // inc guard