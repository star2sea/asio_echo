#include "echo_client.h"

typedef boost::shared_ptr<EchoClient> EchoClientPtr;

int main() {
	io_service service;
	char* echo_msgs[2] = {"hello", "world"}


	for (int i = 0; i < 2; ++i) {
		EchoClientPtr client_ptr(new EchoClient(service));
		client_ptr.start(echo_msgs[i]);
	}
	
	service.run();

	return 0;
}