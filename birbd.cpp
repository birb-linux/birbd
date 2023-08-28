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
		std::cout << "Missing args" << std::endl;
	}

	return 0;
}
