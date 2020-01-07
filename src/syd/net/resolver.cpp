// resolver.cpp                                                       -*-c++-*-
#include <resolver.h>

namespace syd {
namespace net {

// CREATORS
resolver::resolver();

// MANIPULATORS
std::future<address_info_list> resolver::lookup(family             family,
                                                type               type,
                                                std::string const &node,
                                                std::string const &service)
{

  return std::async([family, type, node{std::move(node)}, service{std::move(service)}]()
	     {
	       addrinfo *list = nullptr;

	       addrinfo hints = {0};
	       hints.ai_family = family;
	       hints.ai_socktype = type;
	       hints.ai_flags = AI_ADDRCONFIG | AI_V4MAPPED;
	       
	       int res = getaddrinfo(node.c_str(), service.c_str(), hints, &list);
	       if (0 != res) {
		 // IGNORE ERROR.
	       }

	       return address_info_list(list);	       
	     });

}

} // namespace net
} // namespace syd
