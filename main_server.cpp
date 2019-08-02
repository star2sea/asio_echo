#include "connection.h"
#include "boost/system/error_code.hpp"
using namespace boost::asio;
typedef boost::shared_ptr<Connection> ConnectionPtr;
typedef boost::system::error_code error_code;

io_service service;
ip::tcp::acceptor acceptor(service, ip::tcp::endpoint(ip::tcp::v4(), 8001));

void handle_accept(ConnectionPtr &conn, const error_code &err) {
	conn->start();
	ConnectionPtr new_conn(new Connection(service));
	acceptor.async_accept(conn->sock(), boost::bind(handle_accept, new_conn, _1));
}

int main() {
	ConnectionPtr conn(new Connection(service));
	acceptor.async_accept(conn->sock(), boost::bind(handle_accept, conn, _1));
	service.run();
}