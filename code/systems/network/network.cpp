#include "network.hpp"
#include "network_connection.hpp"
#include <assert.h>

#include <iostream>


namespace Network {


enum class Event_id : uint8_t
{
  none,
  packet_recived,
};


bool
initialize(std::ostream *log)
{
  if(enet_initialize())
  {
    (*log) << "Network failed to initialize in 'Network::initialize'\n";
    return false;
  }
  
  return true;
}


bool
destroy_connection(Connection *connection, std::ostream *log)
{
  // Param check
  if(!connection || !connection->host)
  {
    assert(false);
    
    if(!connection && log)
    {
      (*log) << "Paramater 'connection' cannot be null in 'Network::server_destroy'\n";
    }
    
    if(connection && !connection->host && log)
    {
      (*log) << "Not a valid host connection in 'Network::server_destroy'\n";
    }
    
    return false;
  }

  enet_host_destroy(connection->host);
  return true;
}


bool
server_create(Connection *connection, std::ostream *log)
{
  // Param check
  if(!connection)
  {
    assert(false); // failed param check.
    
    if(log && !connection)
    {
      (*log) << "Cannot pass a null 'connection' to 'Network::server_create'\n";
    }
    
    return false;
  }

  connection->address.host  = ENET_HOST_ANY;
  connection->address.port  = 1234;
  connection->host          = enet_host_create(&connection->address, 4, 2, 0, 0);
  
  if(!connection->host)
  {
    assert(false);
    
    if(log)
    {
      (*log) << "Failed to create server in 'Network::server_create'\n";
    }
    
    return false;
  }
  
  return true;
}


bool
client_create(Connection *connection, std::ostream *log)
{
  // Param check
  if(!connection)
  {
    assert(false); // failed param check.
    
    if(log && !connection)
    {
      (*log) << "Cannot pass a null 'connection' to 'Network::client_create'\n";
    }
    
    return false;
  }

  connection->host = enet_host_create(NULL, 1, 2, 57600 / 8, 14400 / 8);

  if(!connection->host)
  {
    if(log)
    {
      (*log) << "Failed to create client in 'Network::client_create'\n";
    }
    
    return false;
  }

  return true;
}


bool
client_connect_to_server(Connection *connection, const char *ip, const uint32_t port, const uint32_t timeout, std::ostream *log)
{
  // Param check
  if(!connection || !ip)
  {
    assert(false);
    
    if(log && !connection)
    {
      (*log) << "Connection must not be null in 'Network::client_connect_to_server'\n";
    }
    if(log && !ip)
    {
      (*log) << "IP must not be null in 'Network::client_connect_to_server'\n";
    }
    
    return false;
  }

  enet_address_set_host (&connection->address, ip);
  connection->address.port = port;
  /* Initiate the connection, allocating the two channels 0 and 1. */
  connection->peer = enet_host_connect(connection->host, &connection->address, 2, 0);
  if(connection->peer == nullptr)
  {
    if(log)
    {
      (*log) << "No available peers in for connection in 'Network::client_connect_to_server'\n";
    }
    
    return false;
  }

  ENetEvent event;
  
  /* Wait up to 5 seconds for the connection attempt to succeed. */
  if(enet_host_service (connection->host, & event, timeout) > 0 &&
      event.type == ENET_EVENT_TYPE_CONNECT)
  {
    if(log)
    {
      (*log) << "'Network::client_connect_to_server' Connected\n";
    }
    
    return true;
  }
  else
  {
    /* Either the 5 seconds are up or a disconnect event was */
    /* received. Reset the peer in the event the 5 seconds   */
    /* had run out without any significant event.            */
    enet_peer_reset(connection->peer);
    
    if(log)
    {
      (*log) << "'Network::client_connect_to_server' Failed\n";
    }
    
    return false;
  }
  
  return false;
}


bool
send_packet(Connection *connection, const std::size_t size_of_packet, const void *data, const bool reliable, std::ostream *log)
{
  // Param check.
  if(!connection || !connection->peer)
  {
 //   assert(false);
  
    if(log && !connection)
    {
      (*log) << "connection must be valid in 'Network::send_packet'\n";
    }
    if(log && !connection->peer)
    {
      (*log) << "peer not set in connection in 'Network::send_packet'\n";
    }
    
    return false;
  }

  const enet_uint32 packet_type = reliable ? ENET_PACKET_FLAG_RELIABLE : ENET_PACKET_FLAG_UNSEQUENCED;

  ENetPacket * packet = nullptr;
  packet = enet_packet_create(data, size_of_packet, packet_type);

  enet_peer_send(connection->peer, 0, packet);

  return true;
}


void
poll_events(Connection *connection,
            uint32_t wait_ms,
            const std::function<void(const Event_id, void *data, std::size_t data_size)> &callback,
            std::ostream *log)
{
  if(!connection)
  {
    assert(false);
    
    if(log && !connection)
    {
      (*log) << "Not a valid connection on 'Network::poll_events'";
    }
    
    return;
  }

  ENetEvent event;

  //while (enet_host_service(connection->host, & event, static_cast<enet_uint32>(wait_ms)) > 0)
  while(false)
  {
      switch (event.type)
      {
        case ENET_EVENT_TYPE_CONNECT:
        {
          if(log)
          {
            (*log) << "A new client connected from " << event.peer->address.host << ":" << event.peer->address.port;
          }
          
          /* Store any relevant client information here. */
          event.peer -> data = nullptr;
          connection->peer = event.peer;
          
          break;
        }
        
        case ENET_EVENT_TYPE_RECEIVE:
        {
          //printf ("A packet of length %u containing %s was received from %s on channel %u.\n",
          //        event.packet -> dataLength,
          //        event.packet -> data,
          //        event.peer -> data,
          //        event.channelID);

          if(callback)
          {
            callback(Event_id::packet_recived, event.packet->data, event.packet->dataLength);
          }

          /* Clean up the packet now that we're done using it. */
          enet_packet_destroy (event.packet);
          
          break;
        }
         
        case ENET_EVENT_TYPE_DISCONNECT:
        {
          printf ("%s disconnected.\n", event.peer -> data);
          /* Reset the peer's client information. */
          event.peer -> data = NULL;
          break;
        }
        
        default:
          break; // Just to stop warning.
        
      }
  }
}


} // ns