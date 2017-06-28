#include "scene.h"
#include <list>
#include "SFML/OpenGL.hpp"

/** PrintingRegister **/

PrintingRegister::PrintingRegister() : m_objects() {}

void
PrintingRegister::collect(SceneObject * object) {
	m_objects.push_back(object);
}

void
PrintingRegister::discard(SceneObject * object) {
	m_objects.remove(object);
}

std::list<SceneObject *> &
PrintingRegister::getObjects() {
	return m_objects;
}

/** Scene Object **/

SceneObject::SceneObject(PrintingRegister * p_register) :
	m_x(0), m_y(0), m_z(0), m_x_angle(0), m_y_angle(0), m_z_angle(0), m_scale(1),
	m_register(p_register) {
	m_metadata.addClass<SceneObject>();
	m_register->collect(this);
}

SceneObject::SceneObject(PrintingRegister * p_register,
						 float x, float y, float z) :
	m_x(x), m_y(y), m_z(z), m_x_angle(0), m_y_angle(0), m_z_angle(0), m_scale(1),
	m_register(p_register) {
	m_metadata.addClass<SceneObject>();
	m_register->collect(this);
}

SceneObject::~SceneObject() {}


float SceneObject::getX() { return m_x; }
float SceneObject::getY() { return m_y; }
float SceneObject::getZ() { return m_z; }

void SceneObject::setX(float x) { m_x = x; }
void SceneObject::setY(float y) { m_y = y; }
void SceneObject::setZ(float z) { m_z = z; }

float SceneObject::getXAngle() { return m_x_angle; }
float SceneObject::getYAngle() { return m_y_angle; }
float SceneObject::getZAngle() { return m_z_angle; }

void SceneObject::setXAngle(float x) { m_x_angle = x; }
void SceneObject::setYAngle(float y) { m_y_angle = y; }
void SceneObject::setZAngle(float z) { m_z_angle = z; }

float SceneObject::getScale() { return m_scale; }
void SceneObject::setScale(float s) { m_scale = s; }

void
SceneObject::placeObject() {
	glPushMatrix();
		glTranslatef(m_x, m_y, m_z);
		glRotatef(m_x_angle, 1.f, 0.f, 0.f);
		glRotatef(m_y_angle, 0.f, 1.f, 0.f);
		glRotatef(m_z_angle, 0.f, 0.f, 1.f);
		glScalef(m_scale, m_scale, m_scale);
		drawObject();
	glPopMatrix();
}

/** Scene **/

Scene::Scene(PrintingRegister * printing_register, InputManager & inputManager) :
	m_distance (DEFAULT_SCENE_DISTANCE),
	m_scale       (DEFAULT_SCENE_SCALE),
	m_x_angle   (DEFAULT_SCENE_X_ANGLE),
	m_y_angle   (DEFAULT_SCENE_Y_ANGLE),
	m_camera                         (),
	m_register      (printing_register),
	m_controller(*this, inputManager) {

    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
	glClearDepth(1.f);
    glClearColor(0.3f, 0.3f, 0.3f, 0.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDepthFunc(GL_LESS);
}

Scene::Scene(PrintingRegister * printing_register, InputManager & inputManager,
	  float distance, float scale, float x_angle, float y_angle) :
	m_distance           (distance),
	m_scale                 (scale),
	m_x_angle             (x_angle),
	m_y_angle             (y_angle),
	m_camera                     (),
	m_register  (printing_register),
	m_controller(*this, inputManager) {

	glClearDepth(1.f);
    glClearColor(0.3f, 0.3f, 0.3f, 0.f);
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
}

float
Scene::getXAngle() {
	return m_x_angle;
}

float
Scene::getYAngle() {
	return m_y_angle;
}

void
Scene::setXAngle(float x) {
	if( x > MAX_X_ANGLE ) x = MAX_X_ANGLE;
	else if( x < MIN_X_ANGLE ) x = MIN_X_ANGLE;
	m_x_angle = x;
}

void
Scene::setYAngle(float y) {
	m_y_angle = y;
}

void
Scene::placeScene() {

	m_camera.placeCamera();

    glMatrixMode(GL_MODELVIEW);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();

	glTranslatef(0.f, 0.f, - m_distance);

	glScalef(m_scale, m_scale, 1);

	glRotatef(m_y_angle, 1.f, 0.f, 0.f);
	glRotatef(m_x_angle, 0.f, 1.f, 0.f);

	for(std::list<SceneObject *>::iterator i = m_register->getObjects().begin();
		i != m_register->getObjects().end();
		i++) {
		(*i)->placeObject();
	}
}

/** SceneMouseController **/

SceneMouseController::SceneMouseController(Scene & scene, InputManager & manager)
 : m_scene(scene), m_moving(false), m_x_anchor(0), m_y_anchor(0) {
	manager.addCollector(*this);
}

void
SceneMouseController::collect(sf::Event & e) {
	if(e.type == sf::Event::MouseButtonPressed) {
		m_moving = true;
		m_x_anchor = e.mouseButton.x;
		m_y_anchor = e.mouseButton.y;
		m_anchor_x_angle = m_scene.getXAngle();
		m_anchor_y_angle = m_scene.getYAngle();
		return;
	}

	if( ! m_moving ) return;

	if(e.type != sf::Event::MouseMoved) return;

	int delta_x = e.mouseMove.x - m_x_anchor;
	int delta_y = e.mouseMove.y - m_y_anchor;

	m_scene.setXAngle(m_anchor_x_angle + delta_y);
	m_scene.setYAngle(m_anchor_y_angle + delta_x);
}

void
SceneMouseController::discard(sf::Event & e) {}
