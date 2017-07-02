#ifndef CAMERA__H
#define CAMERA__H

class Camera {
	public :
		virtual void placeCamera() = 0;
};

/**
* Représente la caméra : effectue en substance les calculs matriciels permettant
* une projection pseudo-isométrique.
*/
class OrthogonalCamera : public Camera {
	protected :
		float m_left, m_right, m_top, m_bottom, m_near, m_far;
		int m_width, m_height;
	public :
		//static const int DEFAULT_CAMERA_LEFT_LIMIT   =  -100;
		//static const int DEFAULT_CAMERA_RIGHT_LIMIT  =  +100;
		//static const int DEFAULT_CAMERA_TOP_LIMIT    =  -100;
		//static const int DEFAULT_CAMERA_BOTTOM_LIMIT =  +100;
		static const int DEFAULT_CAMERA_NEAR_LIMIT   =    +1;
		static const int DEFAULT_CAMERA_FAR_LIMIT    =  +200;

		OrthogonalCamera(int screen_width, int screen_height);
		OrthogonalCamera(float near, float far,
			  			 int screen_width, int screen_height);

		void setDimensions(int width, int height);

		void placeCamera();
};

#endif
