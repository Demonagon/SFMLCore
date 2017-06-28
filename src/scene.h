#ifndef SCENE__H
#define SCENE__H
#include "camera.h"
#include "metadata.h"
#include "index.h"
#include "collector.h"
#include "input.h"
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
* Controlleur qui permet de déplacer la caméra autour de la scène, à la souris.
* (En vérité c'est la scène qui tourne)
*/

class Scene;

class SceneMouseController : public Collector<sf::Event &> {
	protected :
		Scene & m_scene;
		bool m_moving;
		int m_x_anchor;
		int m_y_anchor;
		float m_anchor_x_angle;
		float m_anchor_y_angle;
	public :
		SceneMouseController(Scene & scene, InputManager & manager);
		void collect(sf::Event & e);
		void discard(sf::Event & e);
};

/**
* Représente la scène, a pour tâche de placer la caméra, puis de se déplacer
* au centre de la vue, puis d'afficher chaque objet.
*/
class Scene {
	protected :
		float m_distance, m_scale, m_x_angle, m_y_angle;
		OrthogonalCamera m_camera;
		PrintingRegister * m_register;
		SceneMouseController m_controller;
	public :
		static const int DEFAULT_SCENE_DISTANCE = +100;
		static const int DEFAULT_SCENE_SCALE    =   +1;
		static const int DEFAULT_SCENE_X_ANGLE  =  +45;
		static const int DEFAULT_SCENE_Y_ANGLE  =  +20;
		static const int MAX_X_ANGLE 			=  +80;
		static const int MIN_X_ANGLE 			=  -80;

		Scene(PrintingRegister * printing_register, InputManager & inputManager);
		Scene(PrintingRegister * printing_register, InputManager & inputManager,
			  float distance, float scale, float x_angle, float y_angle);

		float getXAngle();
		float getYAngle();
		void setXAngle(float x);
		void setYAngle(float y);

		void placeScene();
};

#endif
