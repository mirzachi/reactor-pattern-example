#ifndef IPCCLIENT_H	
#define IPCCLIENT_H

#include <thread>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include "reactor/reactor.h"

namespace ipcclient {


class IPCClient: public reactor::EventHandler
{
public:
	IPCClient(reactor::Reactor& reactor, std::string local_address);
	virtual ~IPCClient ();

	/**
	* \brief Starts IPCClient 
   	*/
  	void start();
  	/**
   	* \brief Stops IPCClient 
   	*/
  	void stop();


private:
	/* Log context */
	const std::string log = "IPCClient";
	/**
   	* \brief A file descriptor representing a Unix domain socket file
   	*/
  	int handle_;

	/**
   	* \brief A name of a socket file representing the local address of the Unix domain socket
   	*/
	std::string local_address_;

	/**
	* \brief A flag to control the client processing thread
	*/
	std::atomic<bool> ipcclient_done_;

  	/**
   	* \brief A reactor which is used for notifications about non-blocking writing to the Unix domain socket
   	*/
  	reactor::Reactor& reactor_;

};

} 

#endif
