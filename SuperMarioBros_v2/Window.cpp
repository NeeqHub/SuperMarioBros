#include "Window.h"

Window::Window(const std::string& windowName) : window(sf::VideoMode(1920,1080), windowName, sf::Style::Default)
{
	window.setVerticalSyncEnabled(true);
}

void Window::update()
{
	sf::Event event;
	if (window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			window.close();
		}
	}
}

void Window::beginDraw()
{
	window.clear(sf::Color::Black);
}

void Window::draw(const sf::Drawable & drawable)
{
	window.draw(drawable);
}

void Window::endDraw()
{
	window.display();
}

bool Window::isOpen() const
{
	return window.isOpen();
}

const sf::View& Window::GetView() const
{
	return window.getView();
}

void Window::SetView(const sf::View& view)
{
	window.setView(view);
}