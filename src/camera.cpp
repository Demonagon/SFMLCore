#include "camera.h"
#include "SFML/OpenGL.hpp"
#include <iostream>

OrthogonalCamera::OrthogonalCamera(int screen_width, int screen_height) :
	//m_left  (DEFAULT_CAMERA_LEFT_LIMIT  ),
	//m_right (DEFAULT_CAMERA_RIGHT_LIMIT ),
	//m_top   (DEFAULT_CAMERA_TOP_LIMIT   ),
	//m_bottom(DEFAULT_CAMERA_BOTTOM_LIMIT),
	m_near  (DEFAULT_CAMERA_NEAR_LIMIT  ),
	m_far   (DEFAULT_CAMERA_FAR_LIMIT   ),
	m_width (screen_width				),
	m_height(screen_height				) {}
	
OrthogonalCamera::OrthogonalCamera(//float left, float right,
						 		   //float top,  float bottom,
						 		   float near, float far,
			  					   int screen_width, int screen_height) :
	//m_left  (left		  ),
	//m_right (right		  ),
	//m_top   (top		  ),
	//m_bottom(bottom		  ),
	m_near  (near		  ),
	m_far   (far   		  ),
	m_width (screen_width ),
	m_height(screen_height) {}

void
OrthogonalCamera::setDimensions(int width, int height) {
	m_width = width;
	m_height = height;
}

void
OrthogonalCamera::placeCamera() {
	glMatrixMode(GL_PROJECTION);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glOrtho(-  m_width/2, +  m_width/2 +  m_width % 2,
			- m_height/2, + m_height/2 + m_height % 2,
			m_near, m_far);
	glViewport(0, 0, m_width, m_height);
}
