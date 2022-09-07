#pragma once
#include <string>
#include <vector>

#include <SFML/Network/Packet.hpp>


struct Message
{
	std::string sender;
	std::string message;

	[[nodiscard]] std::string ToString() const;
	static std::string GetMessagesString(const std::vector<Message>& messages, bool reverse = true);
};

sf::Packet& operator <<(sf::Packet& packet, const Message& m);
sf::Packet& operator >>(sf::Packet& packet, Message& m);