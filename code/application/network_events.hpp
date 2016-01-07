#ifndef NETWORK_EVENTS_INCLUDED_6F116871_D52F_40EA_A5CE_6D237FBD3A72
#define NETWORK_EVENTS_INCLUDED_6F116871_D52F_40EA_A5CE_6D237FBD3A72


namespace Network_event {


namespace Header {
enum ENUM {

  transform_baseline,
  transform_delta,

};
} // ns


struct Transform_sync
{
  float rotation[4];
  float scale[3];
  float position[3];
}; //


} // ns


#endif // inc guard