#include "camera.h"
#include "SFML/OpenGL.hpp"

OrthogonalCamera::OrthogonalCamera() :
	m_left  (DEFAULT_CAMERA_LEFT_LIMIT  ),
	m_right (DEFAULT_CAMERA_RIGHT_LIMIT ),
	m_top   (DEFAULT_CAMERA_TOP_LIMIT   ),
	m_bottom(DEFAULT_CAMERA_BOTTOM_LIMIT),
	m_near  (DEFAULT_CAMERA_NEAR_LIMIT  ),
	m_far   (DEFAULT_CAMERA_FAR_LIMIT   ) {}
	
OrthogonalCamera::OrthogonalCamera(float left, float right,
						 		   float top,  float bottom,
						 		   float near, float far) :
	m_left  (left  ),
	m_right (right ),
	m_top   (top   ),
	m_bottom(bottom),
	m_near  (near  ),
	m_far   (far   ) {}

void OrthogonalCamera::placeCamera() {
	glMatrixMode(GL_PROJECTION);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	//glFrustum(-100, +100, -100, +100, +1, +1000);
	glOrtho(m_left, m_right, m_top, m_bottom, m_near, m_far);
	//glOrtho(-100.f, +100.f, -100.f, +100.f, +1.f, +1000.f);
}
