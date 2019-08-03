#include "echo_client.h"
#include "boost/bind.hpp"
#include <iostream>
#include <algorithm>

using namespace boost::asio;

EchoClient::~EchoClient() {
	stop();
}

void EchoClient::start(const std::string &echo_msg, const std::string &server_ip, short server_port) {
	started_ = true;
	this->echo_msg = echo_msg;
	ip::tcp::endpoint ep(ip::address::from_string(server_ip), server_port);
	sock_.async_connect(ep, boost::bind(&EchoClient::on_connected, shared_from_this(), _1));
}

void EchoClient::stop() {

	if (started_) {
		started_ = false;
		sock_.close();
	}
}

size_t EchoClient::read_complete(const error_code &err, size_t bytes) {
	if (err)
		return 0;

	bool found = std::find(recv_buf, recv_buf + bytes, '\n') < recv_buf + bytes;

	return found ? 0 : 1;
}

void EchoClient::do_read() {
	async_read(sock_, buffer(recv_buf),
		boost::bind(&EchoClient::read_complete, shared_from_this(), _1, _2), 
		boost::bind(&EchoClient::on_read, shared_from_this(), _1, _2));
}

void EchoClient::do_write() {
	std::string msg = echo_msg + '\n';
	std::copy(msg.begin(), msg.end(), send_buf);
	sock_.async_write_some(buffer(send_buf, msg.size()), boost::bind(&EchoClient::on_write, shared_from_this(), _1, _2));
}

void EchoClient::on_read(const error_code &err, size_t bytes) {
	if (err) {
		stop();
		return;
	}
	std::string ret(recv_buf, bytes - 1);
	std::cout << "echo msg " << ret << (ret == echo_msg ? " OK" : " FAILED") << std::endl;
}

void EchoClient::on_write(const error_code &err, size_t bytes) {
	if (err) {
		stop();
		return;
	}
	do_read();
}


void EchoClient::on_connected(const error_code &err) {
	if (err) {
		stop();
		return;
	}
	do_write();
}