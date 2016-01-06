#ifndef NETWORK_CONNECTION_INCLUDED_9F83E032_AE96_4D9A_B6D4_6AC7121523EC
#define NETWORK_CONNECTION_INCLUDED_9F83E032_AE96_4D9A_B6D4_6AC7121523EC


#include <enet/enet.h>


namespace Network {


struct Connection
{
  ENetAddress address;
  ENetHost *host = nullptr;
  ENetPeer *peer = nullptr;
}; // Connection


} // ns


#endif // inc guard