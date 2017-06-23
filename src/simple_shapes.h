#ifndef SIMPLE_SHAPES__H
#define SIMPLE_SHAPES__H

#include <SFML/Graphics.hpp>
#include "collector.h"
#include "provider.h"
#include <list>
/**
C'est un carré qui permet un affichage rapide et simple de fonctionnalités.
Le seul artiste fait pour fonctionner avec cette classe est le SimpleArtist.
**/

class SimpleRectangle;

class SimpleRectangleChangeEvent {
	public :
		SimpleRectangle & rectangle;
		double new_x, new_y, new_width, new_height;
		sf::Color new_color;
};

class SimpleRectangle : public StandardListProvider<SimpleRectangleChangeEvent> {
	protected :
		double m_x, m_y, m_width, m_height;
		sf::Color m_color;
	public :
		SimpleRectangle(double x, double y, double w, double h,
						sf::Color color);

		SimpleRectangleChangeEvent generateChangeEvent();

		double getX();
		double getY();
		double getW();
		double getH();
		sf::Color getColor();
		void setX(double x);
		void setY(double y);
		void setW(double w);
		void setH(double h);
		void setColor(sf::Color c);
	
		void actualizeGraphics();
		
};

class SimpleRectanglePainter : public Collector<SimpleRectangle &> {};

#endif
