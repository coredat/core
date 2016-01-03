#ifndef NETWORK_INCLUDED_0EA72B07_517F_432F_8BE5_B568C1EF197E
#define NETWORK_INCLUDED_0EA72B07_517F_432F_8BE5_B568C1EF197E


#include "network_fwd.hpp"
#include <cstddef>


namespace Network {


bool initialize();
bool de_initalize();

bool server_create(Connection *connection);
bool client_create(Connection *connection);
bool client_connect_to_server(Connection *connection, const char *ip);
bool send_packet(Connection *connection, const std::size_t size_of_packet, const void *data, const bool reliable);
void poll_events();


bool setup_network();
bool setup_as_client();
bool setup_as_server();
bool connect_to_server(const char *str);
void poll_events();
bool send_rel_packet();
bool send_unrel_packet();
void destroy_network();


} // ns


#endif // inc guard