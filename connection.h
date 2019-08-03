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

class Connection : public boost::enable_shared_from_this<Connection>, boost::noncopyable {
public:
	Connection(boost::asio::io_service &service) :sock_(service) {}
	~Connection();

	void start();
	void stop();

	boost::asio::ip::tcp::socket &sock() { return sock_; }

private:
	typedef boost::system::error_code error_code;
	bool started_;
	char send_buf[1024];
	char recv_buf[1024];
	boost::asio::ip::tcp::socket sock_;

	size_t read_complete(const error_code &err, size_t bytes);
	void do_read();
	void do_write(std::string &echo_msg);
	void on_read(const error_code &err, size_t bytes);
	void on_write(const error_code &err, size_t bytes);
};
