#ifndef HOST_INC
#define HOST_INC


#include <string>


bool setup_network();
bool setup_as_client();
bool setup_as_server();
bool connect_to_server(const std::string &str);
void poll_events();
bool send_rel_packet();
bool send_unrel_packet();
void destroy_network();


#endif