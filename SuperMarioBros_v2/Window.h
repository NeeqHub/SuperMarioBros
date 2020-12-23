#pragma once
#include <SFML/Graphics.hpp>
#include <array>
#include <iostream>
#include <functional>

class Window
{
public:
	Window(const std::string& windowName);

	void update();
	
	void beginDraw();
	void draw(const sf::Drawable& drawable);
	void endDraw();

	bool isOpen() const;

	const sf::View& GetView() const;
	void SetView(const sf::View& view);

	// TO delete
	void Draw(const sf::Vertex* vertices,
		std::size_t vertexCount, sf::PrimitiveType type);

private:
	sf::RenderWindow window;
};

/*class Debug
{
public:
	static void Draw(Window& window);

	static void DrawRect(const sf::FloatRect& rect,
		sf::Color colour = sf::Color::White);

	static void DrawLine(const sf::Vector2f& from,
		const sf::Vector2f& to,
		sf::Color colour = sf::Color::White);

	static void Log(const std::string& msg);
	static void LogWarning(const std::string& msg);
	static void LogError(const std::string& msg);

private:
	static std::vector<std::array<sf::Vertex, 2>> lines;
	static std::vector<sf::RectangleShape> rects;
};*/
