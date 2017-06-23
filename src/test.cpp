#include "test.h"
#include "terrain.h"
#include "input.h"
#include "SFML/Window.hpp"

#include "simple_artist.h"
#include "simple_shapes.h"
#include "collector.h"

class SimpleRectangleController : public Collector<sf::Event &> {
	public :
		SimpleRectangle & r;
		InputManager & m;
		SimpleArtist & a;

		SimpleRectangleController(
			SimpleRectangle & rr,
			InputManager & mm,
			SimpleArtist & aa) : r(rr), m(mm), a(aa) {
			m.addCollector(*this);
		}

		void collect(sf::Event & e) {
		    if (e.type == sf::Event::Closed) {
		      	a.close();
				m.interruptLoop();
				return;
			}

			if(e.type == sf::Event::MouseMoved) {
				r.setX( e.mouseMove.x );
				r.setY( e.mouseMove.y );
				r.actualizeGraphics();
				a.paint();
			}
		}

		void discard(sf::Event & e) {}
};

int main()
{
    /*sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);

	Terrain terrain(100, 100);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(shape);
        window.display();
    }*/

	/*Terrain terrain(80, 80);
	TerrainVisualizer visualizer(terrain);
	visualizer.display();*/

	SimpleArtist artist(500, 500, sf::Color(50, 50, 200));

	SimpleRectangle r1(100, 100, 10, 10, sf::Color(0, 255, 0));
	SimpleRectangle r2(100, 200, 100, 10, sf::Color(255, 255, 0));

	artist.collect(r1);
	artist.collect(r2);

	artist.paint();

	InputManager m(artist.getWindow(), artist.getWindowLock());

	SimpleRectangleController c(r2, m, artist);

	artist.getWindow().setActive(false);

	m.inputLoop();

    return 0;
}
