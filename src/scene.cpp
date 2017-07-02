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

Scene::Scene(sf::RenderWindow & window,
			 PrintingRegister * printing_register,
			  int screen_width, int screen_height) :
	m_window					  (window),
	m_view(sf::Rect<float>(0.f, 0.f, screen_width, screen_height)),
	m_distance    (DEFAULT_SCENE_DISTANCE),
	m_scale          (DEFAULT_SCENE_SCALE),
	m_x_angle      (DEFAULT_SCENE_X_ANGLE),
	m_y_angle      (DEFAULT_SCENE_Y_ANGLE),
	m_scale_lock	 			        (),
	m_angle_lock					    (),
	m_camera (screen_width, screen_height),
	m_register         (printing_register) {

	m_window.setView(m_view);

    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
	glClearDepth(1.f);
    glClearColor(0.3f, 0.3f, 0.3f, 0.f);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glDepthFunc(GL_LESS);
}

Scene::Scene(sf::RenderWindow & window,
			 PrintingRegister * printing_register,
	  float distance, float scale, float x_angle, float y_angle,
	  int screen_width, int screen_height) :
	m_window					  			(window),
	m_view(sf::Rect<float>(0.f, 0.f, screen_width, screen_height)),
	m_distance             				  (distance),
	m_scale                        			 (scale),
	m_x_angle               	 		   (x_angle),
	m_y_angle               	 		   (y_angle),
	m_scale_lock	 			   				  (),
	m_angle_lock	 			   				  (),
	m_camera 		   (screen_width, screen_height),
	m_register   	   			 (printing_register) {
	m_window.setView(m_view);

    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
	glClearDepth(1.f);
    glClearColor(0.3f, 0.3f, 0.3f, 0.f);
}

float
Scene::getScale() {
	return m_scale;
}

void
Scene::setScale(float s) {
	m_scale = s;
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

OrthogonalCamera &
Scene::getCamera() {
	return m_camera;
}
		
sf::RenderWindow &
Scene::getWindow() {
	return m_window;
}

sf::View &
Scene::getView() {
	return m_view;
}

void
Scene::setView(sf::View view) {
	m_view = view;
}

void
Scene::placeScene() {

	m_camera.placeCamera();

    glMatrixMode(GL_MODELVIEW);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

	glTranslatef(0.f, 0.f, - m_distance);

	glRotatef(m_x_angle, 1.f, 0.f, 0.f);
	glRotatef(m_y_angle, 0.f, 1.f, 0.f);

	glScalef(m_scale, m_scale, 1);

	for(std::list<SceneObject *>::iterator i = m_register->getObjects().begin();
		i != m_register->getObjects().end();
		i++) {
		(*i)->placeObject();
	}
}
