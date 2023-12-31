#include "Crypto.hpp"
#include "Server.hpp"
#include <array>
#include <chrono>
#include <exception>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <thread>
#include <vector>

using namespace std::chrono_literals;

const std::string distcache_path = "/var/cache/distfiles";

Server::Server(int port)
:acceptor(boost::asio::ip::tcp::acceptor(io_context, tcp::endpoint(tcp::v4(), port))),
	port(port)
{}

std::vector<std::string> split_string(std::string text, const std::string& delimiter)
{
	assert(text.empty() == false);
	assert(delimiter.empty() == false);

	std::vector<std::string> result;

	/* Split the string */
	size_t pos = 0;
	while ((pos = text.find(delimiter)) != std::string::npos)
	{
		result.push_back(text.substr(0, pos));
		text.erase(0, pos + delimiter.length());
	}

	if (!text.empty())
		result.push_back(text);

	return result;
}


void Server::listen()
{
	std::cout << "Listening in port " << port << std::endl;

	try
	{
		while (true)
		{
			tcp::socket socket(io_context);
			acceptor.accept(socket);

			std::cout << "Connection accepted from " << socket.remote_endpoint() << "\n";

			/* Read data from the socket */
			std::string received_data;

			constexpr int buffer_size = 128;

			/* How many times will the server accept a full buffer worth
			 * of data. 8 * 128 = 1024 chars */
			constexpr int max_buffer_receive_count = 8;
			int full_buffer_counter = 0;

			while (true)
			{
				std::array<char, buffer_size> buf;
				buf.fill(0);

				boost::system::error_code error;
				size_t len = socket.read_some(boost::asio::buffer(buf), error);

				if (error == boost::asio::error::eof)
					break;
				else if (error)
					throw boost::system::system_error(error);

				std::cout << "Got " << len << " bytes of data\n";
				received_data += buf.data();
				full_buffer_counter++;

				/* We have read plenty of data. If this if-statement gets hit, this
				 * is probably a DOS attempt or a malformed connection */
				if (full_buffer_counter >= max_buffer_receive_count)
					break;
			}
			socket.close();


			/* Answer the client */
			acceptor.accept(socket);
			std::string answer = "";

			/* Ignore requests that are too short or long */
			if (received_data.size() < 4 || received_data.size() > 1024)
				answer = "NULL";
			else
				answer = message_handler(received_data);

			const char* raw_answer = reinterpret_cast<const char*>(answer.data());
			boost::asio::write(socket, boost::asio::buffer(raw_answer, answer.size()), ignored_error);

			std::this_thread::sleep_for(10ms);
		}
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
}

bool Server::file_exists(const std::string& file_path) const
{
	return std::filesystem::exists(file_path) && std::filesystem::is_regular_file(file_path);
}

bool Server::contains_path_traversal(const std::string& file_name) const
{
	return (file_name.find("..") != std::string::npos || file_name.find("/") != std::string::npos);
}

std::string Server::message_handler(std::string message)
{
	std::cout << "Message: " << message << std::endl;

	/* Define all queries */
	constexpr char ping[] 			= "ping";
	constexpr char has_package[] 	= "has_package";

	/* Define all answers */
	constexpr char positive[] 	= "positive";
	constexpr char negative[] 	= "negative";
	constexpr char pong[] 		= "pong";
	constexpr char null[] 		= "NULL";

	/* Remove the last DEL character */
	if (message[message.size() - 1] == 127)
		message.erase(message.size() - 1, 1);

	/* Split the message into parts */
	std::vector<std::string> message_parts = split_string(message, ";");

	/* Connection test */
	if (message_parts[0] == ping)
		return pong;

	/* Download or check if the server has a package distfile */
	if (message_parts[0] == has_package && message_parts.size() == 3)
	{
		/* Prevent path traversal vulnerabilities */
		std::string tarball = message_parts[1];
		if (contains_path_traversal(tarball))
			return negative;

		const std::string tarball_path 		= distcache_path + "/" + tarball;
		const std::string tarball_checksum 	= message_parts[2];

		if (file_exists(tarball_path))
		{
			/* File was found, check if the checksum matches */
			std::string md5 = get_file_hash(tarball_path);

			if (md5 == tarball_checksum)
				return positive;
			else
				return negative;
		}
		else
		{
			/* Package file wasn't found */
			return negative;
		}
	}

	return null;
}
