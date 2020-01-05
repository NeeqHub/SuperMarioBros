#pragma once
#include <SFML/Graphics.hpp>

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

private:
	sf::RenderWindow window;
};

