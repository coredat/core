#ifndef MESH_RENDERER_INCLUDED_4307B221_E5ED_4329_B7DC_6AE309DCF1AF
#define MESH_RENDERER_INCLUDED_4307B221_E5ED_4329_B7DC_6AE309DCF1AF


namespace Core {


class Mesh_renderer
{
public:

  explicit              Mesh_renderer();
  
  void                  set_material(const int mesh);
  int                   get_material() const;

  void                  set_mesh(const int mesh);
  int                   get_mesh() const;
  
  void                  render(); // Move to the world process.
  
private:

}; // class


} // ns


#endif // inc guard