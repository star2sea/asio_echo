#pragma once

//#include "boost/asio/io_service.hpp"
//#include "boost/asio/ip/tcp.hpp"
//#include "boost/asio/ip/address.hpp"
#include "boost/asio.hpp"
#include "boost/enable_shared_from_this.hpp"
#include "boost/noncopyable.hpp"
#include "boost/shared_ptr.hpp"
#include "boost/system/error_code.hpp"
#include <string>

class EchoClient : public boost::enable_shared_from_this<EchoClient>, boost::noncopyable {
public:
	EchoClient(boost::asio::io_service &service) :sock_(service) {}
	~EchoClient();
	void start(const std::string &echo_msg, const std::string &server_ip, short server_port);
	void stop();

private:
	typedef boost::system::error_code error_code;
	bool started_;
	std::string echo_msg;
	char send_buf[1024];
	char recv_buf[1024];
	boost::asio::ip::tcp::socket sock_;

	size_t read_complete(const error_code &err, size_t bytes);
	void do_read();
	void do_write();
	void on_read(const error_code &err, size_t bytes);
	void on_write(const error_code &err, size_t bytes);
	void on_connected(const error_code &err);
};

