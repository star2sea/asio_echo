#include "echo_client.h"
#include <iostream>
#include <algorithm>

using namespace boost::asio;

EchoClient::~EchoClient() {
	stop();
}

void EchoClient::start(std::string &echo_msg, const char* ip, short port) {
	started_ = true;
	echo_msg = echo_msg;
	ip::tcp::endpoint ep(ip::address::from_string(ip), port);
	sock_.async_connect(ep, boost::bind(&EchoClient::on_connected, shared_from_this(), _1);
}

void EchoClient::stop() {
	if (started_) {
		started_ = false;
		sock_.close();
	}
}

size_t EchoClient::read_complete(error_code &err, size_t bytes) {
	if (err)
		return 0;

	bool found = std::find(read_buf, read_buf + bytes, '\n') < read_buf + bytes;

	return found ? 0 : 1;
}

void EchoClient::do_read() {
	async_read(sock_, buffer(read_buf), 
		boost::bind(&EchoClient::read_complete, shared_from_this(), _1, _2), 
		boost::bind(&EchoClient::on_read, shared_from_this(), _1, _2));
}

void EchoClient::do_write() {
	std::string msg = echo_msg + '\n';
	std::copy(msg.begin(), msg.end(), send_buf);
	sock_.async_write_some(buffer(send_buf, echo_msg.size()), boost::bind(&EchoClient::on_write, shared_from_this(), _1, _2))
}

void EchoClient::on_read(error_code &err, size_t bytes) {
	if (err) {
		stop();
		return;
	}
	std::string ret(read_buf, bytes - 1);
	std::cout << "echo msg " << ret << ret == echo_msg ? " OK" : " FAILED" << std::endl;
}

void EchoClient::on_write(error_code &err, size_t bytes) {
	if (err) {
		stop();
		return;
	}
	do_read();
}


void EchoClient::on_connected(error_code &err) {
	if (err) {
		stop();
		return;
	}
	do_write();
}