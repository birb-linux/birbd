#include "Client.hpp"
#include <exception>
#include <iostream>
#include <array>
#include <string>

Client::Client(std::string host, std::string port)
{
	tcp::resolver resolver(io_context);
	endpoints = resolver.resolve(host, port);
}

std::string Client::connect(const std::string& data)
{
	std::string response;

	if (data.size() < 4)
	{
		std::cerr << "You must send at least 4 bytes of data\n";
		return "NULL";
	}

	try
	{
		/* Open a socket for sending data */
		tcp::socket socket(io_context);
		boost::asio::connect(socket, endpoints);

		/* Send data to the server */
		boost::asio::write(socket, boost::asio::buffer(data), ignored_error);
		socket.close();

		/* Open a new socket for receiving data */
		boost::asio::connect(socket, endpoints);

		/* Wait for a response */
		while (true)
		{
			std::array<char, 128> buf;
			boost::system::error_code error;

			size_t len = socket.read_some(boost::asio::buffer(buf), error);

			if (error == boost::asio::error::eof)
				break;
			else if (error)
				throw boost::system::system_error(error);

			response += buf.data();
		}
		socket.close();
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}

	return response;
}
