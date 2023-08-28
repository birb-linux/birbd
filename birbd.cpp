#include "Client.hpp"
#include "Server.hpp"
#include <iostream>

int main(int argc, char** argv)
{
	/* Act as a server */
	if (argc == 2 && !strcmp("--server", argv[1]))
	{
		Server server(1500);
		server.listen();
		return 0;
	}

	/* Send a message to the server */
	if (argc == 3)
	{
		Client client(argv[1], "1500");
		std::string response = client.connect(argv[2]);
		std::cout << response << std::endl;

		/* If the response was NULL, return 1
		 * in all other cases return 0 */
		if (response == "NULL")
			return 1;
		else
			return 0;
	}
	else
	{
		std::cout 	<< "Usage: birbd [ip address] [--server|message]\n\n"
					<< "If --server option is used, birbd will act as a server. IP-address isn't needed in server-mode\n\n"
					<< "Valid messages (values between [] are placeholders):\n"
					<< "  ping                                       the server should answer with pong\n"
					<< "  has_package;[tarball_name];[md5_checksum]  the server will answer either positive or negative\n"
					<< "                                             depending on if the server had the tarball or not\n\n"
					<< "If the server answers NULL, something went wrong or the query was invalid\n";
	}

	return 0;
}
