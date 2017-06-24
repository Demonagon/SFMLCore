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
	m_x(0), m_y(0), m_z(0), m_x_angle(0), m_y_angle(0), m_z_angle(0),
	m_register(p_register) {
	m_metadata.addClass<SceneObject>();
	m_register->collect(this);
}

SceneObject::SceneObject(PrintingRegister * p_register,
						 float x, float y, float z) :
	m_x(x), m_y(y), m_z(z), m_x_angle(0), m_y_angle(0), m_z_angle(0),
	m_register(p_register) {
	m_metadata.addClass<SceneObject>();
	m_register->collect(this);
}

SceneObject::~SceneObject() {}

void
SceneObject::placeObject() {
	glPushMatrix();
		glTranslatef(m_x, m_y, m_z);
		glRotatef(m_x_angle, 1.f, 0.f, 0.f);
		glRotatef(m_y_angle, 0.f, 1.f, 0.f);
		glRotatef(m_z_angle, 0.f, 0.f, 1.f);
		drawObject();
	glPopMatrix();
}

/** Scene **/

Scene::Scene(PrintingRegister * printing_register) :
	m_distance (DEFAULT_SCENE_DISTANCE),
	m_scale       (DEFAULT_SCENE_SCALE),
	m_x_angle   (DEFAULT_SCENE_X_ANGLE),
	m_y_angle   (DEFAULT_SCENE_Y_ANGLE),
	m_camera                         (),
	m_register      (printing_register) {

	glClearDepth(1.f);
    glClearColor(0.3f, 0.3f, 0.3f, 0.f);
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
}

Scene::Scene(PrintingRegister * printing_register,
	  float distance, float scale, float x_angle, float y_angle) :
	m_distance           (distance),
	m_scale                 (scale),
	m_x_angle             (x_angle),
	m_y_angle             (y_angle),
	m_camera                     (),
	m_register  (printing_register) {

	glClearDepth(1.f);
    glClearColor(0.3f, 0.3f, 0.3f, 0.f);
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
}

void
Scene::placeScene() {
	m_camera.placeCamera();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
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
