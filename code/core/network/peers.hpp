#ifndef PEERS_INCLUDED_27A10822_9516_446A_A540_348B47C08321
#define PEERS_INCLUDED_27A10822_9516_446A_A540_348B47C08321


#include <stdint.h>


namespace Core {
namespace Network {


uint32_t
get_number_of_connected_peers();


bool
is_peer_connected(uint32_t peer);


} // ns
} // ns


#endif // inc guard