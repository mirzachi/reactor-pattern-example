#include <iostream>
#include "IPCClient.h"

namespace ipcclient {

/* Reactor not used since this client can write anytime to the used Unix domain socket (no contention) */
IPCClient::IPCClient(reactor::Reactor& reactor, std::string local_address)
	: reactor_(reactor), local_address_(local_address){}
	
IPCClient::~IPCClient (){

	std::cout << log << ": " << "Destructing IPCClient" << std::endl << std::endl;
} 

void IPCClient::start(){
		
	std::cout << log << ": " << "Starting IPCClient" << std::endl << std::endl;	
	/* If Reactor is used here you can register your write handler */
	/* Not a very good use case; it sends all the time */
	//reactor_.register_event_handler(handle_, this, reactor::EventType::kWriteEvent);
	std::thread thread([this](){
		while (1) {

			  if ( (handle_ = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
			    perror("socket error");
			    exit(1);
			  }
			  struct sockaddr_un addr;
			  memset(&addr, 0, sizeof(addr));
			  addr.sun_family = AF_UNIX;
			  strcpy(addr.sun_path, local_address_.c_str());
			  socklen_t lenAddr_=sizeof(addr.sun_family) + strlen(addr.sun_path);

			  if (connect(handle_, (struct sockaddr*)&addr, lenAddr_) != 0 ){ 
				perror("Connection unsuccessful!!!");
				exit(1);
			  }

			  std::cout << log << ": " << "Connected to the IPCServer..." << std::endl;
			  const std::string msg = "Hello from Client";
			  std::cout << log << ": " << "Sending data to server: " << msg << std::endl;
			  if (write(handle_, msg.c_str(), msg.size())!=msg.size()){
				  perror("write");
				  exit(1);
			  }
			  close(handle_);
			  std::this_thread::sleep_for(std::chrono::milliseconds(5000));
		}});
		thread.detach();
}

void IPCClient::stop(){
	std::cout << log << ": " << "Stopping IPCClient" << std::endl << std::endl;
  	close(handle_);
}


}

