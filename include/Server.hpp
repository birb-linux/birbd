#pragma once
#include <boost/asio.hpp>
#include <unordered_map>
#include <string>

using boost::asio::ip::tcp;

class Server
{
public:
	Server(int port);
	void listen();

private:
	std::string message_handler(std::string message);
	bool file_exists(const std::string& file_path) const;
	bool contains_path_traversal(const std::string& file_name) const;
	int port;

	boost::asio::io_context io_context;
	boost::system::error_code ignored_error;
	tcp::acceptor acceptor;
};
