#include "connection.h"
#include <iostream>

using namespace boost::asio;


Connection::~Connection() {
	stop();
}

void Connection::start() {
	started_ = true;
	do_read();
}

void Connection::stop() {
	if (started_) {
		started_ = false;
		sock_.close();
	}
}

size_t Connection::read_complete(error_code &err, size_t bytes) {
	if (err)
		return 0;

	bool found = std::find(read_buf, read_buf + bytes, '\n') < read_buf + bytes;

	return found ? 0 : 1;
}

void Connection::do_read() {
	async_read(sock_, buffer(read_buf),
		boost::bind(&Connection::read_complete, shared_from_this(), _1, _2),
		boost::bind(&Connection::on_read, shared_from_this(), _1, _2));
}

void Connection::do_write(std::string &echo_msg) {
	std::string msg = echo_msg + '\n';
	std::copy(msg.begin(), msg.end(), send_buf);
	sock_.async_write_some(buffer(send_buf, echo_msg.size()), boost::bind(&EchoClient::on_write, shared_from_this(), _1, _2))
}

void Connection::on_read(error_code &err, size_t bytes) {
	if (err) {
		stop();
		return;
	}
	std::string echo_msg(read_buf, bytes-1);
	std::cout << "server on read " << echo_msg << std::endl;
	do_write(echo_msg);
}

void EchoClient::on_write(error_code &err, size_t bytes) {
	
}