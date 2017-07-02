#include "controller.h"

/** Exit Controller **/

ExitController::ExitController(Artist & artist, InputManager & i) :
	m_artist(artist), m_i(i) {
	m_i.addCollector(*this);
}

void
ExitController::collect(sf::Event & e) {
	if (e.type == sf::Event::Closed) {
		m_i.interruptLoop();
		m_artist.stop();
		m_artist.close();
	}
	if ((e.type == sf::Event::KeyPressed) &&
		(e.key.code == sf::Keyboard::Escape)) {
		m_i.interruptLoop();
		m_artist.stop();
		m_artist.close();
	}
}

void
ExitController::discard(sf::Event & e) {}

/** ResizingController **/

ResizingController::ResizingController(Scene & scene, InputManager & i)
	: m_scene(scene) {
	i.addCollector(*this);
}

void
ResizingController::collect(sf::Event & e) {
	if(e.type != sf::Event::Resized)  return;

	m_scene.getCamera().setDimensions(e.size.width,
									  e.size.height);
}

void
ResizingController::discard(sf::Event & e) {}


/** CameraController **/

CameraController::CameraController(Scene & scene, InputManager & i)
 : m_scene(scene), m_moving(false), m_x_anchor(0), m_y_anchor(0) {
	i.addCollector(*this);
}

void
CameraController::collect(sf::Event & e) {
	if(e.type == sf::Event::MouseButtonPressed) {
		m_moving = true;
		m_x_anchor = e.mouseButton.x;
		m_y_anchor = e.mouseButton.y;
		m_anchor_x_angle = m_scene.getXAngle();
		m_anchor_y_angle = m_scene.getYAngle();
		return;
	} else if(e.type == sf::Event::MouseButtonReleased) {
		m_moving = false;
	}

	if( ! m_moving ) return;

	if(e.type != sf::Event::MouseMoved) return;

	int delta_x = e.mouseMove.x - m_x_anchor;
	int delta_y = e.mouseMove.y - m_y_anchor;

	m_scene.setXAngle(m_anchor_x_angle + delta_y);
	m_scene.setYAngle(m_anchor_y_angle + delta_x);
}

void
CameraController::discard(sf::Event & e) {}
