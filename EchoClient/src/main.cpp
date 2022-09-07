#include <imgui.h>
#include <imgui-SFML.h>

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <iostream>

constexpr std::size_t BUFFER_SIZE = 500;

void RenderInputWindow(char* inputBuffer, const std::string& content)
{
	ImGui::SetNextWindowSizeConstraints(ImVec2(500, 300), ImVec2(1000, 1000));
	ImGui::Begin("Chat !");
	ImGui::InputTextWithHint("Chat", "Text to send", inputBuffer, BUFFER_SIZE);
	ImGui::Button("Send");
	ImGui::TextWrapped(content.c_str());
	ImGui::End();
}



int main() {
	sf::RenderWindow window(sf::VideoMode(640 * 2, 480 * 2), "Client");
	window.setFramerateLimit(60);
	if (!ImGui::SFML::Init(window))
	{
		std::cout << "Error loading ImGui" << std::endl;
		return EXIT_FAILURE;
	}

	// Set window scale for HDPI screens
	ImGuiIO& io = ImGui::GetIO();
	io.FontGlobalScale = 2;

	// Initialized text input buffer
	char inputBuffer[BUFFER_SIZE];
	for (char& i : inputBuffer)
	{
		i = 0;
	}

	std::vector<std::string> messages;

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

		RenderInputWindow(inputBuffer, content);

		window.clear();
		ImGui::SFML::Render(window);
		window.display();
	}

	ImGui::SFML::Shutdown();

	return EXIT_SUCCESS;
}