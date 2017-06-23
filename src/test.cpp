#include "test.h"
#include "terrain.h"
#include "input.h"
#include "SFML/Window.hpp"
#include "SFML/OpenGL.hpp"
#include "SFML/Graphics.hpp"

#include "simple_artist.h"
#include "simple_shapes.h"
#include "collector.h"

#include<cmath>

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

//Remplacement à gluPerspective
void perspectiveGL( GLdouble fovY, GLdouble aspect, GLdouble zNear, GLdouble zFar )
{
    const GLdouble pi = 3.1415926535897932384626433832795;
    GLdouble fW, fH;

    //fH = tan( (fovY / 2) / 180 * pi ) * zNear;
    fH = tan( fovY / 360 * pi ) * zNear;
    fW = fH * aspect;

    glFrustum( -fW, fW, -fH, fH, zNear, zFar );
}

void makeProjectionMatrix() {
	glLoadIdentity();
 
	double cavalier[] = {
    	1, 0, cos(M_PI/4), 0,
    	0, 1, sin(M_PI/4), 0,
    	0, 0, 1,           0,
    	0, 0, 0,           1};
 
	glMultTransposeMatrixd(cavalier);
	glOrtho(-100, 100, -100, 100, 1.f, 300.0f);

    //perspectiveGL(90.f, 1.f, 1.f, 300.0f);//fov, aspect, zNear, zFar
}

int isometric_main()
{
    // Create the main window
    sf::RenderWindow App(sf::VideoMode(800, 600, 32), "SFML OpenGL");

    // Create a clock for measuring time elapsed
    sf::Clock Clock;
	
    //prepare OpenGL surface for HSR
	glClearDepth(1.f);
    glClearColor(0.3f, 0.3f, 0.3f, 0.f);
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);

    //// Setup a perspective projection & Camera position
	glMatrixMode(GL_PROJECTION);
	makeProjectionMatrix();

    bool rotate=true;
	float angle;
	
	// Start game loop
	while (App.isOpen())
    {
        // Process events
        sf::Event Event;
        while (App.pollEvent(Event))
        {
            // Close window : exit
            if (Event.type == sf::Event::Closed)
                App.close();

            // Escape key : exit
            if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::Escape))
                App.close();

			if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::A)){
				rotate=!rotate;
			}
 
		}
        
        //Prepare for drawing
		// Clear color and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Apply some transformations for the cube
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glTranslatef(0.f, 0.f, -200.f);
		
		if(rotate){
			angle=Clock.getElapsedTime().asSeconds();
		}
		glRotatef(angle * 50, 1.f, 0.f, 0.f);
		glRotatef(angle * 30, 0.f, 1.f, 0.f);
		glRotatef(angle * 90, 0.f, 0.f, 1.f);
		//TMP mouvement de caméra
		/*glMatrixMode(GL_PROJECTION);
		makeProjectionMatrix();
		glRotatef(angle * 100, 0.f, 1.f, 0.f);
        glMatrixMode(GL_MODELVIEW);*/
			
		
		 
		//Draw a cube
        glBegin(GL_QUADS);//draw some squares
			glColor3i(0,1,1);
            glVertex3f(-50.f, -50.f, -50.f);
            glVertex3f(-50.f,  50.f, -50.f);
            glVertex3f( 50.f,  50.f, -50.f);
            glVertex3f( 50.f, -50.f, -50.f);

			glColor3f(0,0,1);
            glVertex3f(-50.f, -50.f, 50.f);
            glVertex3f(-50.f,  50.f, 50.f);
            glVertex3f( 50.f,  50.f, 50.f);
            glVertex3f( 50.f, -50.f, 50.f);

			glColor3f(1,0,1);
            glVertex3f(-50.f, -50.f, -50.f);
            glVertex3f(-50.f,  50.f, -50.f);
            glVertex3f(-50.f,  50.f,  50.f);
            glVertex3f(-50.f, -50.f,  50.f);

			glColor3f(0,1,0);
            glVertex3f(50.f, -50.f, -50.f);
            glVertex3f(50.f,  50.f, -50.f);
            glVertex3f(50.f,  50.f,  50.f);
            glVertex3f(50.f, -50.f,  50.f);

			glColor3f(1,1,0);
            glVertex3f(-50.f, -50.f,  50.f);
            glVertex3f(-50.f, -50.f, -50.f);
            glVertex3f( 50.f, -50.f, -50.f);
            glVertex3f( 50.f, -50.f,  50.f);

			glColor3f(1,0,0);
            glVertex3f(-50.f, 50.f,  50.f);
            glVertex3f(-50.f, 50.f, -50.f);
            glVertex3f( 50.f, 50.f, -50.f);
            glVertex3f( 50.f, 50.f,  50.f);

        glEnd();

        // Finally, display rendered frame on screen
        App.display();
    }

    return EXIT_SUCCESS;
}

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

	/*SimpleArtist artist(500, 500, sf::Color(50, 50, 200));

	SimpleRectangle r1(100, 100, 10, 10, sf::Color(0, 255, 0));
	SimpleRectangle r2(100, 200, 100, 10, sf::Color(255, 255, 0));

	artist.collect(r1);
	artist.collect(r2);

	artist.paint();

	InputManager m(artist.getWindow(), artist.getWindowLock());

	SimpleRectangleController c(r2, m, artist);

	artist.getWindow().setActive(false);

	m.inputLoop();*/

	return isometric_main();
}
