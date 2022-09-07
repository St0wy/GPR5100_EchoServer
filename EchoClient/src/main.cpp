#include <imgui.h>
#include <imgui-SFML.h>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Network/TcpSocket.hpp>
#include <SFML/Window/Event.hpp>
#include <iostream>
#include <optional>

#include "Consts.h"
#include "Message.h"
#include "SFML/Network/IpAddress.hpp"


void ResetBuffer(char* buffer, const std::size_t size)
{
	for (std::size_t i = 0; i < size; ++i)
	{
		buffer[i] = 0;
	}
}

std::optional<std::string> RenderChatWindow(char* charInputBuffer, const std::string& content)
{
	ImGui::SetNextWindowSizeConstraints(ImVec2(500, 300), ImVec2(1000, 1000));
	ImGui::Begin("Chat !");
	ImGui::Text("Write text and press enter to send it !");
	// ReSharper disable once CppTooWideScope
	const bool pressedEnter = ImGui::InputTextWithHint(
		"##chat",
		"Text to send",
		charInputBuffer,
		MESSAGE_BUFFER_SIZE,
		ImGuiInputTextFlags_EnterReturnsTrue);
	if (pressedEnter)
	{
		ImGui::SetKeyboardFocusHere(-1);
	}

	ImGui::TextWrapped(content.c_str());  // NOLINT(clang-diagnostic-format-security)
	ImGui::End();

	if (pressedEnter)
	{
		std::string inputText(charInputBuffer);
		ResetBuffer(charInputBuffer, MESSAGE_BUFFER_SIZE);
		return inputText;
	}

	return {};
}

bool RenderLoginWindow(
	char* senderInputBuffer, char* ipInputBuffer, char* portInputBuffer,
	std::string& name, sf::IpAddress& ip, unsigned short& port
)
{
	ImGui::SetNextWindowSizeConstraints(ImVec2(500, 300), ImVec2(1000, 1000));
	ImGui::Begin("Login");
	ImGui::TextWrapped("Please type your name, ip of the server and its ports.");
	// ReSharper disable once CppTooWideScope
	ImGui::InputTextWithHint("##name", "Your name", senderInputBuffer, SENDER_BUFFER_SIZE);
	ImGui::InputTextWithHint("##ip", "IP", ipInputBuffer, IP_BUFFER_SIZE);
	ImGui::InputTextWithHint("##port", "Port", portInputBuffer, PORT_BUFFER_SIZE);
	bool isConnecting = false;
	if (ImGui::Button("Login"))
	{
		const std::string inputName(senderInputBuffer);
		const std::string inputIp(ipInputBuffer);
		const unsigned short inputPort = static_cast<unsigned short>(std::stoi(portInputBuffer));
		if (!inputName.empty())
		{
			name = inputName;
			ip = inputIp;
			port = inputPort;
			isConnecting = true;
		}
		else
		{
			std::cout << "Name is too short.\n";
			std::cout << inputName << "\n" << senderInputBuffer << "\n";
			std::cout << name.empty() << "\n";
		}
	}
	ImGui::End();

	return isConnecting;
}

int main()
{
	std::vector<Message> messages;

	sf::RenderWindow window(sf::VideoMode(640 * 2, 480 * 2), "Client");
	if (!ImGui::SFML::Init(window))
	{
		std::cout << "Error loading ImGui" << std::endl;
		return EXIT_FAILURE;
	}

	// Set window scale for HDPI screens
	ImGuiIO& io = ImGui::GetIO();
	io.FontGlobalScale = 2;

	// Create socket
	sf::TcpSocket socket;
	socket.setBlocking(false);

	// Initialized text input buffer
	char messageInputBuffer[MESSAGE_BUFFER_SIZE]{};
	char senderInputBuffer[SENDER_BUFFER_SIZE]{};
	char portInputBuffer[PORT_BUFFER_SIZE] = "8008";
	char ipInputBuffer[IP_BUFFER_SIZE] = "localhost";

	// Session vars
	std::string name;
	sf::IpAddress ip;
	unsigned short port;
	bool isConnected = false;
	bool isSendingMessage = false;
	bool isConnecting = false;
	bool isReceiving = false;
	sf::Packet sendingPacket;
	sf::Packet receivePacket;

	sf::Clock deltaClock;
	while (window.isOpen()) {
		sf::Event event{};
		while (window.pollEvent(event)) {
			ImGui::SFML::ProcessEvent(window, event);

			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
			else if (event.type == sf::Event::Resized)
			{
				sf::Vector2f size = { static_cast<float>(event.size.width), static_cast<float>(event.size.height) };
				sf::Vector2f center = size / 2.0f;
				window.setView(sf::View(center, size));
			}
		}

		ImGui::SFML::Update(window, deltaClock.restart());

		// Read messages from network
		sf::Socket::Status status = socket.receive(receivePacket);
		if (status == sf::Socket::Done)
		{
			Message receiveMessage;
			receivePacket >> receiveMessage;
			messages.push_back(receiveMessage);
		}

		if (isConnected)
		{
			Message sendingMessage;
			auto inputText = RenderChatWindow(messageInputBuffer, Message::GetMessagesString(messages));
			if (inputText.has_value())
			{
				sendingMessage = { name, inputText.value() };
				sendingPacket << sendingMessage;
				isSendingMessage = true;
			}

			if (isSendingMessage)
			{
				sf::Socket::Status status = socket.send(sendingPacket);
				if (status == sf::Socket::Disconnected || status == sf::Socket::Error)
				{
					std::cerr << "Couldn't send message.\n";
				}
				else if (status == sf::Socket::Done)
				{
					std::cout << "Message sent : " << sendingMessage.ToString() << "\n";
					messages.push_back(sendingMessage);
					isSendingMessage = false;
				}
			}
		}
		else
		{
			bool shouldTryToConnect = RenderLoginWindow(senderInputBuffer, ipInputBuffer, portInputBuffer, name, ip, port);

			if (!isConnecting && shouldTryToConnect)
			{
				isConnecting = shouldTryToConnect;
			}

			if (isConnecting)
			{
				sf::Socket::Status status = socket.connect(ip, port, sf::seconds(10.0f));

				if (status == sf::Socket::Disconnected || status == sf::Socket::Error)
				{
					std::cerr << "Error with the connection to the server.\n";
					isConnected = false;
					isConnecting = false;
				}

				if (status == sf::Socket::Done || status == sf::Socket::NotReady)
				{
					isConnected = true;
					isConnecting = false;
					std::cout << "Connection successful !\n";
				}
			}
		}

		window.clear();
		ImGui::SFML::Render(window);
		window.display();
	}

	ImGui::SFML::Shutdown();

	return EXIT_SUCCESS;
}