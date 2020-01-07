// socket.h                                                          -*-c++-*-
#ifndef INCLUDED_SYD_NET_SOCKET
#define INCLUDED_SYD_NET_SOCKET

#include <string>

#include <sys/socket.h>
#include <sys/types.h>

namespace syd {
namespace net {

enum class type : int
{
  Stream = SOCK_STREAM
};

enum class family : int
{
  Stream = SOCK_STREAM
};
  
  
} // namespace net
} // namespace syd

#endif // INCLUDED_SYD_NET_SOCKET
