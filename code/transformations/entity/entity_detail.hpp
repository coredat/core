#ifndef ENTITY_DETAIL_INCLUDED_0D8C8FBE_379D_476A_8C4E_D80363253849
#define ENTITY_DETAIL_INCLUDED_0D8C8FBE_379D_476A_8C4E_D80363253849


namespace Entity_detail {


/*
  Collision Callback
  --
  We hold the function pointer as a uintptr_t because
  at this level we only care about it being data.
  The calling code needs to know this information,
  but the data doesn't.
*/
struct Callback_collision
{
  uintptr_t user_data;
  uintptr_t callback_fn;
};  

  
} // ns


#endif // inc guard