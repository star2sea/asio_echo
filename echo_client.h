#pragma once

#include "boost/asio/io_service.hpp"
#include "boost/enable_shared_from_this.hpp"
#include "boost/noncopyable.hpp"
#include "boost/shared_ptr.hpp"
#include "boost/system/error_code.hpp"
#include <string>

class EchoClient : public boost::enable_shared_from_this<EchoClient>, boost::noncopyable {
public:
	EchoClient(io_service &service) :sock_(service) {}
	~EchoClient();
	void start(std::string &echo_msg, const char* ip, short port);
	void stop();

private:
	typedef boost::system::error_code error_code;
	bool started_;
	std::string echo_msg;
	char send_buf[1024];
	char recv_buf[1024];
	boost::asio::ip::tcp::socket sock_;

	size_t read_complete(error_code &err, size_t bytes);
	void do_read();
	void do_write();
	void on_read(error_code &err, size_t bytes);
	void on_write(error_code &err, size_t bytes);
	void on_connected(error_code &err);
};

