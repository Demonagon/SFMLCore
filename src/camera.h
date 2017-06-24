#ifndef CAMERA__H
#define CAMERA__H

class Camera {
	public :
		virtual void placeCamera() = 0;
};

class OrthogonalCamera : public Camera {
	protected :
		float m_left, m_right, m_top, m_bottom, m_near, m_far;
	public :
		static const int DEFAULT_CAMERA_LEFT_LIMIT   =  -100;
		static const int DEFAULT_CAMERA_RIGHT_LIMIT  =  +100;
		static const int DEFAULT_CAMERA_TOP_LIMIT    =  -100;
		static const int DEFAULT_CAMERA_BOTTOM_LIMIT =  +100;
		static const int DEFAULT_CAMERA_NEAR_LIMIT   =    +1;
		static const int DEFAULT_CAMERA_FAR_LIMIT    = +1000;

		OrthogonalCamera();
		OrthogonalCamera(float left, float right,
						 float top,  float bottom,
						 float near, float far);

		void placeCamera();
};

#endif
