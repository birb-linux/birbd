#pragma once
#include <boost/asio.hpp>
#include <string>

using boost::asio::ip::tcp;

class Client
{
public:
	Client(std::string host, std::string port);
	std::string connect(const std::string& data);

private:
	tcp::resolver::results_type endpoints;
	boost::asio::io_context io_context;
	boost::system::error_code ignored_error;
};
