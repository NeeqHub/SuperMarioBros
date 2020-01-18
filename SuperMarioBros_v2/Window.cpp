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
	window.clear(sf::Color(107, 140, 255));
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

// TO delete
void Window::Draw(const sf::Vertex* vertices,
	std::size_t vertexCount, sf::PrimitiveType type)
{
	window.draw(vertices, vertexCount, type);
}

std::vector<sf::RectangleShape> Debug::rects = {};
std::vector<std::array<sf::Vertex, 2>> Debug::lines = {};

void Debug::Draw(Window& window)
{
	for (auto& r : rects)
	{
		window.draw(r);
	}
	rects.clear();

	for (auto& l : lines)
	{
		sf::Vertex line[2] = { l.at(0), l.at(1) };
		window.Draw(line, 2, sf::Lines);
	}
	lines.clear();
}

void Debug::DrawRect(const sf::FloatRect& rect, sf::Color colour)
{
	sf::Vector2f size(rect.width, rect.height);
	sf::Vector2f pos(rect.left, rect.top);
	sf::RectangleShape shape(size);
	shape.setPosition(pos);
	shape.setOutlineColor(colour);
	shape.setOutlineThickness(3.f);
	shape.setFillColor(sf::Color::Transparent);
	rects.push_back(shape);
}

void Debug::DrawLine(const sf::Vector2f& from,
	const sf::Vector2f& to, sf::Color colour)
{
	lines.push_back({ sf::Vertex(from, colour),
					 sf::Vertex(to, colour) });
}

void Debug::Log(const std::string& msg)
{
	std::cout << msg << std::endl;
}

void Debug::LogWarning(const std::string& msg)
{
	std::cout << "WARNING: " << msg << std::endl;
}

void Debug::LogError(const std::string& msg)
{
	std::cout << "ERROR: " << msg << std::endl;
}

/*sf::FloatRect Window::GetViewSpace() const
{
	const sf::View& view = GetView();
	const sf::Vector2f& viewCenter = view.getCenter();
	const sf::Vector2f& viewSize = view.getSize();
	sf::Vector2f viewSizeHalf(viewSize.x * 0.5f, viewSize.y * 0.5f);
	sf::FloatRect viewSpace(viewCenter - viewSizeHalf, viewSize);
	return viewSpace;
}*/