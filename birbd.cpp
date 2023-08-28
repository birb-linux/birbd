#include "Client.hpp"
#include "Server.hpp"
#include <iostream>

int main(int argc, char** argv)
{
	if (argc == 2)
	{
		/* Act as a server */
		if (!strcmp("--server", argv[1]))
		{
			Server server(1500);
			server.listen();
			return 0;
		}

		/* Send a message to the server */
		Client client("localhost", "1500");
		std::string response = client.connect(argv[1]);
		std::cout << response << std::endl;
	}
	else
	{
		std::cout 	<< "Usage: birbd [--server|message]\n\n"
					<< "If --server option is used, birbd will act as a server\n\n"
					<< "Valid messages (values between [] are placeholders):\n"
					<< "  ping                                       the server should answer with pong\n"
					<< "  has_package;[tarball_name];[md5_checksum]  the server will answer either positive or negative\n"
					<< "                                             depending on if the server had the tarball or not\n\n"
					<< "If the server answers NULL, something went wrong or the query was invalid\n";
	}

	return 0;
}
