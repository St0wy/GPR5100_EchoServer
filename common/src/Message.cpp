#include "Message.h"

#include <format>

std::string Message::ToString() const
{
	return std::format("[{}] {}", sender, message);
}

std::string Message::GetMessagesString(const std::vector<Message>& messages, const bool reverse)
{
	std::string text;
	if (reverse)
	{
		const int size = static_cast<int>(messages.size());
		for (int i = size - 1; i >= 0; --i)
		{
			text += messages[i].ToString() + "\n";
		}
	}
	else
	{
		for (auto& message : messages)
		{
			text += message.ToString() + "\n";
		}
	}

	return text;
}

sf::Packet& operator<<(sf::Packet& packet, const Message& m)
{
	return packet << m.sender << m.message;
}

sf::Packet& operator>>(sf::Packet& packet, Message& m)
{
	return packet >> m.sender >> m.message;
}
