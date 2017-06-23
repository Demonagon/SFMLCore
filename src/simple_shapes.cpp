#include "simple_shapes.h"

SimpleRectangle::SimpleRectangle(double x, double y, double w, double h,
								 sf::Color color) : m_x(x), m_y(y),
	m_width(w), m_height(h), m_color(color) {}


SimpleRectangleChangeEvent
SimpleRectangle::generateChangeEvent() {
	return (SimpleRectangleChangeEvent) {
			.rectangle = *this,
			.new_x = m_x, .new_y = m_y,
			.new_width = m_width, .new_height = m_height,
			.new_color = m_color
		};
}

double
SimpleRectangle::getX() {
	return m_x;
}

double
SimpleRectangle::getY() {
	return m_y;
}

double
SimpleRectangle::getW() {
	return m_width;
}

double
SimpleRectangle::getH() {
	return m_height;
}

sf::Color
SimpleRectangle::getColor() {
	return m_color;
}

void
SimpleRectangle::setX(double x) {
	m_x = x;
}

void
SimpleRectangle::setY(double y) {
	m_y = y;
}

void
SimpleRectangle::setW(double w) {
	m_width = w;
}

void
SimpleRectangle::setH(double h) {
	m_height = h;
}

void
SimpleRectangle::setColor(sf::Color c) {
	m_color = c;
}
	
void
SimpleRectangle::actualizeGraphics() {
	SimpleRectangleChangeEvent e = generateChangeEvent();
	provide( e );
}
