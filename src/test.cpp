#include "test.h"
#include "terrain.h"
#include "input.h"
#include "gradiant.h"
#include "SFML/Window.hpp"
#include "SFML/OpenGL.hpp"
#include "SFML/Graphics.hpp"

#include "simple_artist.h"
#include "simple_shapes.h"
#include "collector.h"
#include "scene.h"
#include "collector.h"
#include "input.h"
#include "artist.h"

#include <cmath>

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
 
	/*double cavalier[] = {
    	1, 0, cos(M_PI), 0,
    	0, 1, sin(M_PI/16), 0,
    	0, 0, 1,           0,
    	0, 0, 0,           1};
 
	glMultTransposeMatrixd(cavalier);*/
	//glRotatef(45, 0.f, 1.f, 0.f);
	//glRotatef(10, 1.f, 0.f, 0.f);
	glOrtho(-100, +100, -100, +100, 1.f, 600.0f);

    //perspectiveGL(90.f, 1.f, 1.f, 300.0f);//fov, aspect, zNear, zFar
}

int isometric_main()
{
    // Create the main window

	sf::ContextSettings context(24, 8, 2, 3, 3);
    sf::RenderWindow App(sf::VideoMode(800, 600, 32), "SFML OpenGL", 7U, context);

    // Create a clock for measuring time elapsed
    sf::Clock Clock;
	
    //prepare OpenGL surface for HSR
	glClearDepth(1.f);
    glClearColor(0.3f, 0.3f, 0.3f, 0.f);
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);

	sf::ContextSettings windowSettings = App.getSettings();
	std::cout
	<< "windowSettings.DepthBits: " << windowSettings.depthBits << "\n"
	<<  "windowSettings.StencilBits: " << windowSettings.stencilBits << "\n"
	<< "windowSettings.AntialiasingLevel: " << windowSettings.antialiasingLevel << "\n"
	<< "windowSettings.MajorVersion: " << windowSettings.majorVersion << "\n"
	<< "windowSettings.MinorVersion: " << windowSettings.minorVersion << "\n";

    //// Setup a perspective projection & Camera position
	glMatrixMode(GL_PROJECTION);
	makeProjectionMatrix();

    bool rotate=true;
	float angle;
	float other_angle;
	//float z = -300.f;
	//float x = 0.f;
	
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
		
		if(rotate){
			//z = -300.f - (((int) Clock.getElapsedTime().asSeconds()) % 100);
			angle= Clock.getElapsedTime().asSeconds();
			other_angle = cos(Clock.getElapsedTime().asSeconds());
		}
			angle=Clock.getElapsedTime().asSeconds();
		//x = Clock.getElapsedTime().asSeconds() -
		//	10 * ( (int) Clock.getElapsedTime().asSeconds() / 10 );
        glTranslatef(0.f, 0.f, -300.f);

		double scale_value = ( Clock.getElapsedTime().asSeconds() -
			3 * ( (int) Clock.getElapsedTime().asSeconds() / 3 ) ) / 3.0;
		if(scale_value <= 0.1) scale_value = 0;
		double scale = Gradiant::polGradiant(1, 0, scale_value, 2);
		glScalef(scale, scale, 1);

		glRotatef(other_angle * 50, 1.f, 0.f, 0.f);
		glRotatef(angle * 45, 0.f, 1.f, 0.f);
		//glRotatef(20, 1.f, 0.f, 0.f);
		//glRotatef(45, 0.f, 1.f, 0.f);
			
		
		 
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

class Cube : public SceneObject {
	private :
		float m_red, m_green, m_blue;
	public :
		Cube(float red, float green, float blue,
			 PrintingRegister * p_register) :
			SceneObject(p_register),
			m_red(red), m_green(green), m_blue(blue) {}

		virtual void drawObject() {
			glBegin(GL_QUADS);

			glColor3f(m_red   * 1.f,
					  m_green * 1.f,
					  m_blue  * 1.f);
		    glVertex3f(-50.f, -50.f, -50.f);
		    glVertex3f(-50.f,  50.f, -50.f);
		    glVertex3f( 50.f,  50.f, -50.f);
		    glVertex3f( 50.f, -50.f, -50.f);

			glColor3f(m_red   * .9f,
					  m_green * .9f,
					  m_blue  * .9f);
		    glVertex3f(-50.f, -50.f, 50.f);
		    glVertex3f(-50.f,  50.f, 50.f);
		    glVertex3f( 50.f,  50.f, 50.f);
		    glVertex3f( 50.f, -50.f, 50.f);

			glColor3f(m_red   * .8f,
					  m_green * .8f,
					  m_blue  * .8f);
		    glVertex3f(-50.f, -50.f, -50.f);
		    glVertex3f(-50.f,  50.f, -50.f);
		    glVertex3f(-50.f,  50.f,  50.f);
		    glVertex3f(-50.f, -50.f,  50.f);

			glColor3f(m_red   * .7f,
					  m_green * .7f,
					  m_blue  * .7f);
		    glVertex3f(50.f, -50.f, -50.f);
		    glVertex3f(50.f,  50.f, -50.f);
		    glVertex3f(50.f,  50.f,  50.f);
		    glVertex3f(50.f, -50.f,  50.f);

			glColor3f(m_red   * .6f,
					  m_green * .6f,
					  m_blue  * .6f);
		    glVertex3f(-50.f, -50.f,  50.f);
		    glVertex3f(-50.f, -50.f, -50.f);
		    glVertex3f( 50.f, -50.f, -50.f);
		    glVertex3f( 50.f, -50.f,  50.f);

			glColor3f(m_red   * .5f,
					  m_green * .5f,
					  m_blue  * .5f);
		    glVertex3f(-50.f, 50.f,  50.f);
		    glVertex3f(-50.f, 50.f, -50.f);
		    glVertex3f( 50.f, 50.f, -50.f);
		    glVertex3f( 50.f, 50.f,  50.f);

			glEnd();
		}
};

class Controller : public Collector<sf::Event&> {
	private :
		Artist & m_artist;
		InputManager & m_i;

	public :
		Controller(Artist & artist, InputManager & i) :
			m_artist(artist), m_i(i) {
			m_i.addCollector(*this);
		}

		void collect(sf::Event & e) {
			if (e.type == sf::Event::Closed) {
				m_i.interruptLoop();
				m_artist.stop();
				m_artist.close();
			}
			if ((e.type == sf::Event::KeyPressed) && (e.key.code == sf::Keyboard::Escape)) {
				m_i.interruptLoop();
				m_artist.stop();
				m_artist.close();
			}
		}

		void discard(sf::Event & e) {}
};

int scene_main() {
	Artist artist(1200, 400);
	InputManager manager(artist.getWindow(), artist.getWindowLock() );
	artist.setInputManager(manager);

	Cube cube1(1.f, 0.f, 0.f, &artist.getPrintingRegister());
	Cube cube2(0.f, 1.f, 0.f, &artist.getPrintingRegister());
	Cube cube3(0.f, 0.f, 1.f, &artist.getPrintingRegister());

	cube2.setX(60);
	cube3.setZ(60);

	cube1.setScale(.5f);
	cube2.setScale(.5f);
	cube3.setScale(.5f);

	Controller controller(artist, manager);

	artist.start();
	manager.inputLoop();

	/*sf::ContextSettings context(24, 8, 2, 3, 3);
    sf::RenderWindow window(sf::VideoMode(800, 600, 32), "SFML OpenGL", 7U, context);
	MutexDataLock window_lock;

	InputManager inputManager(window, window_lock);

	Cube cube1(1.f, 0.f, 0.f, &reg);
	Cube cube2(0.f, 1.f, 0.f, &reg);
	Cube cube3(0.f, 0.f, 1.f, &reg);

	cube2.setX(60);
	cube3.setZ(60);

	cube1.setScale(.5f);
	cube2.setScale(.5f);
	cube3.setScale(.5f);

	std::atomic<bool> end(false);

	Controller controller(window, end, inputManager);

	inputManager.addCollector( controller );

	scene.placeScene();
	window.display();
	
	window.setActive(false);

	inputManager.inputLoop();*/

	/*scene.placeScene();
	window.display();*/

	//while(! end) {}

	/*while (window.isOpen())
    {
        sf::Event Event;
        while (window.waitEvent(Event))
        {
 
		}
    }*/

	return 0;
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

	artist.start();

	m.inputLoop();*/

	return scene_main();
}
