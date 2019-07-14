#include <iostream>
#include "IPCServer.h"

namespace ipcserver {

IPCServer::IPCServer(reactor::Reactor& reactor, std::string local_address)
		: reactor_(reactor), local_address_(local_address){

  if ( (handle_ = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
    perror("socket error");
    exit(1);
  }

  struct sockaddr_un addr;
  memset(&addr, 0, sizeof(addr));
  addr.sun_family = AF_UNIX;
  strcpy(addr.sun_path, local_address.c_str());


  if (bind(handle_, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
    perror("bind error");
    exit(1);
  }

  if (listen(handle_, 1) == -1) {
    perror("listen error");
    exit(1);
  }
		
}
	
IPCServer::~IPCServer (){

	std::cout << log << ": " << "Destructing IPCServer" << std::endl << std::endl;
	unlink(local_address_.c_str());
	//close();
} 

/**
 * Process some other things while waiting on new connections
*/
void IPCServer::do_something_else(){
	std::thread thread = std::thread([this](){
	while(!ipcserver_done_){
		std::cout << log << ": " << "I am not blocked, I process some other stuff..." << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		
	}});
	thread.detach();
}

/**
* \brief Starts reading messages from the Unix domain socket connected to a server.
*/
void IPCServer::start(){
		
	std::cout << log << ": " << "Starting IPCServer" << std::endl << std::endl;	
	reactor_.register_event_handler(handle_, this, reactor::EventType::kReadEvent);
	do_something_else();
	

}

/**
* \brief Stops reading messages from the Unix domain socket connected to a server
*        and closes the connection.
*/
void IPCServer::stop(){
	std::cout << log << ": " << "Stopping IPCServer" << std::endl << std::endl;
	ipcserver_done_ = true;
}


/**
* \brief handler for asynchronous event notification on the Unix domain socket connected to a server.
*
* \param a an a argument.
*/
void IPCServer::handle_read(int handle) {
	std::thread thread = std::thread([this]() {
      
		int cl, rc;
		char buf[100];
		std::cout << log << ": " << "handle_read called" << std::endl;
		if ( (cl = accept(handle_, NULL, NULL)) == -1) {
			std::cout << log << ": " << "Accept error" << std::endl;
		}
		std::cout << log << ": " << "New connection accepted" << std::endl;

		if ( (rc=read(cl,buf,sizeof(buf))) > 0) {
			printf("%s: Read %u bytes: %.*s\n\n", log.c_str(), rc, rc, buf);
		}

		close(cl);
    	});
	thread.join();
} 

}

