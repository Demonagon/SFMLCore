#ifndef SCENE__H
#define SCENE__H
#include "camera.h"
#include "metadata.h"
#include "index.h"
#include "collector.h"
#include "input.h"
#include <SFML/Graphics.hpp>
#include <list>

class SceneObject;


/**
* Cette classe retient quels sont les objets qui sont à afficher. C'est un
* contournement du système d'indexer qui pose des problèmes difficiles de 
* cast ; tout problème similaire devra être résolu de cette façon, de là
* où je vois les choses.
*/
class PrintingRegister : public Collector<SceneObject *> {
	protected :
		std::list<SceneObject *> m_objects;
	public :
		PrintingRegister();

		virtual void collect(SceneObject * object);
		virtual void discard(SceneObject * object);

		std::list<SceneObject *> & getObjects();
};

/**
* Représente un objet qui doit être affiché à l'écran. La prise en compte
* des coordonnées dans l'espace ainsi que de la rotation est prise en compte.
*/
class SceneObject : public MetaDataObject {
	protected :
		float m_x, m_y, m_z, m_x_angle, m_y_angle, m_z_angle, m_scale;
		PrintingRegister * m_register;
	public :
		SceneObject(PrintingRegister * p_register);
		SceneObject(PrintingRegister * p_register,
					float x, float y, float z);
		virtual ~SceneObject();

		float getX();
		float getY();
		float getZ();
		void setX(float x);
		void setY(float y);
		void setZ(float z);
		float getXAngle();
		float getYAngle();
		float getZAngle();
		void setXAngle(float x);
		void setYAngle(float y);
		void setZAngle(float z);
		float getScale();
		void setScale(float s);

		virtual void placeObject();
		virtual void drawObject() = 0;
};

/**
* Représente la scène, a pour tâche de placer la caméra, puis de se déplacer
* au centre de la vue, puis d'afficher chaque objet.
*/
class Scene {
	protected :
		sf::RenderWindow & m_window;
		sf::View m_view;
		float m_distance, m_scale, m_x_angle, m_y_angle;
		MutexDataLock m_scale_lock;
		MutexDataLock m_angle_lock;
		OrthogonalCamera m_camera;
		PrintingRegister * m_register;
	public :
		static const int DEFAULT_SCENE_DISTANCE = +100;
		static const int DEFAULT_SCENE_SCALE    =   +1;
		static const int DEFAULT_SCENE_X_ANGLE  =  +45;
		static const int DEFAULT_SCENE_Y_ANGLE  =  +20;
		static const int MAX_X_ANGLE 			=  +90;
		static const int MIN_X_ANGLE 			=  -90;

		Scene(sf::RenderWindow & window,
			  PrintingRegister * printing_register,
			  int screen_width, int screen_height);
		Scene(sf::RenderWindow & window,
			  PrintingRegister * printing_register,
			  float distance, float scale, float x_angle, float y_angle,
			  int screen_width, int screen_height);

		float getScale();
		void setScale(float s);

		float getXAngle();
		float getYAngle();
		void setXAngle(float x);
		void setYAngle(float y);

		OrthogonalCamera & getCamera();
		sf::RenderWindow & getWindow();
		sf::View & getView();
		void setView(sf::View view);

		void placeScene();
};

#endif
