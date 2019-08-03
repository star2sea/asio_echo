#include "echo_client.h"

typedef boost::shared_ptr<EchoClient> EchoClientPtr;

int main() {
	boost::asio::io_service service;
	
	char* echo_msgs[2] = { "hello", "world" };

	for (int i = 0; i < 2; ++i) {
		EchoClientPtr client_ptr(new EchoClient(service));
		client_ptr->start(echo_msgs[i], "127.0.0.1", 8001);
	}
	
	service.run();

	printf("client over\n");

	getchar();

	return 0;
}