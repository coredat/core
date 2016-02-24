#ifndef NETWORK_INCLUDED_0EA72B07_517F_432F_8BE5_B568C1EF197E
#define NETWORK_INCLUDED_0EA72B07_517F_432F_8BE5_B568C1EF197E


#include "network_fwd.hpp"
#include <cstddef>
#include <ostream>
#include <stdint.h>
#include <functional>


namespace Network {


bool initialize(std::ostream *log = nullptr);

bool server_create(Connection *connection, std::ostream *log = nullptr);
bool client_create(Connection *connection, std::ostream *log = nullptr);
bool destroy_connection(Connection *connection, std::ostream *log = nullptr);

bool client_connect_to_server(Connection *connection, const char *ip, const uint32_t port, const uint32_t timeout, std::ostream *log = nullptr);
bool send_packet(Connection *connection, const std::uint32_t size_of_packet, const void *data, const bool reliable, std::ostream *log = nullptr);

void poll_events(Connection *connection,
                 uint32_t wait_ms,
                 const std::function<void(Event_id id, void *data, std::uint32_t data_size)> &callback,
                 std::ostream *log = nullptr);


} // ns


#endif // inc guard