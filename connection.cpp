#include "connection.h"
#include "boost/bind.hpp"
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

size_t Connection::read_complete(const error_code &err, size_t bytes) {
	if (err)
		return 0;


	bool found = std::find(recv_buf, recv_buf + bytes, '\n') < (recv_buf + bytes);


	return found ? 0 : 1;
}

void Connection::do_read() {
	async_read(sock_, buffer(recv_buf),
		boost::bind(&Connection::read_complete, shared_from_this(), _1, _2),
		boost::bind(&Connection::on_read, shared_from_this(), _1, _2));
}

void Connection::do_write(std::string &echo_msg) {
	std::string msg = echo_msg + '\n';
	std::copy(msg.begin(), msg.end(), send_buf);
	sock_.async_write_some(buffer(send_buf, msg.size()), boost::bind(&Connection::on_write, shared_from_this(), _1, _2));
}

void Connection::on_read(const error_code &err, size_t bytes) {
	if (err) {
		stop();
		return;
	}
	std::string echo_msg(recv_buf, bytes-1);
	std::cout << "server on read " << echo_msg << std::endl;
	do_write(echo_msg);
}

void Connection::on_write(const error_code &err, size_t bytes) {
	
}