#ifndef SCENE__H
#define SCENE__H
#include "camera.h"
#include "metadata.h"
#include "index.h"
#include "collector.h"
#include <list>

class SceneObject;

class PrintingRegister : public Collector<SceneObject *> {
	protected :
		std::list<SceneObject *> m_objects;
	public :
		PrintingRegister();

		virtual void collect(SceneObject * object);
		virtual void discard(SceneObject * object);

		std::list<SceneObject *> & getObjects();
};

class SceneObject : public MetaDataObject {
	protected :
		float m_x, m_y, m_z, m_x_angle, m_y_angle, m_z_angle;
		PrintingRegister * m_register;
	public :
		SceneObject(PrintingRegister * p_register);
		SceneObject(PrintingRegister * p_register,
					float x, float y, float z);
		virtual ~SceneObject();

		virtual void placeObject();
		virtual void drawObject() = 0;
};

class Scene {
	protected :
		float m_distance, m_scale, m_x_angle, m_y_angle;
		OrthogonalCamera m_camera;
		PrintingRegister * m_register;
	public :
		static const int DEFAULT_SCENE_DISTANCE = +500;
		static const int DEFAULT_SCENE_SCALE    =   +1;
		static const int DEFAULT_SCENE_X_ANGLE  =  +45;
		static const int DEFAULT_SCENE_Y_ANGLE  =  +20;

		Scene(PrintingRegister * printing_register);
		Scene(PrintingRegister * printing_register,
			  float distance, float scale, float x_angle, float y_angle);

		void placeScene();
};

#endif
