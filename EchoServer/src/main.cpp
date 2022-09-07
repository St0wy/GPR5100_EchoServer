#include <iostream>
#include <SFML/Network/TcpListener.hpp>
#include <SFML/Network/TcpSocket.hpp>
#include <SFML/Network/SocketSelector.hpp>

#include "Consts.h"
#include "Message.h"

void ListenSockets(const std::vector<std::unique_ptr<sf::TcpSocket>>& clients, const sf::SocketSelector& selector)
{
	std::vector<Message> messages;
	for (auto& client : clients)
	{
		if (!selector.isReady(*client)) continue;

		// The client has some data
		// ReSharper disable once CppTooWideScopeInitStatement
		sf::Packet packet;
		if (client->receive(packet) == sf::Socket::Done)
		{
			std::cout << "Listening received data.\n";
			Message message;
			packet >> message;
			messages.push_back(message);

			std::cout << message.ToString() << "\n";

			for (auto& messageReceiver : clients)
			{
				if (client == messageReceiver) continue;

				if (messageReceiver->send(packet) != sf::Socket::Done)
				{
					std::cerr << "Problem when propagating the message\n";
				}
				else
				{
					std::cout << "Sent message !\n";
				}
			}
		}
	}
}

int main()
{
	sf::TcpListener listener;
	if (listener.listen(SERVER_PORT) != sf::Socket::Done)
	{
		std::cerr << "Can't listen.\n";
		return EXIT_FAILURE;
	}

	std::vector<std::unique_ptr<sf::TcpSocket>> clients;

	sf::SocketSelector selector;
	selector.add(listener);

	std::cout << "Starting server...\n";

	while (true)
	{
		// Make the selector wait for data on any socket
		if (selector.wait(sf::seconds(10.0f)))
		{
			if (selector.isReady(listener))
			{
				std::cout << "Listening for a connection...\n";
				// The listener is ready, there is a connection
				// ReSharper disable once CppTooWideScopeInitStatement
				auto client = std::make_unique<sf::TcpSocket>();
				if (listener.accept(*client) == sf::Socket::Done)
				{
					std::cout << "Connection success !\n";
					selector.add(*client);
					clients.push_back(std::move(client));
				}
			}
			else
			{
				// The listener socket is not ready, test all other sockets (the clients)
				ListenSockets(clients, selector);
			}
		}
	}

	return EXIT_SUCCESS;
}
