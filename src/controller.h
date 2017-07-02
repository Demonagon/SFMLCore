#ifndef CONTROLLER__H
#define CONTROLLER__H
#include "input.h"
#include "scene.h"
#include "collector.h"
#include "artist.h"

/**
* Controleur qui s'occupe de fermer la fenêtre quand on appuie sur la croix où
* que l'on appuie sur échap.
*/

class ExitController : public Collector<sf::Event&> {
	private :
		Artist & m_artist;
		InputManager & m_i;

	public :
		ExitController(Artist & artist, InputManager & i);

		void collect(sf::Event & e);

		void discard(sf::Event & e);
};

/**
* Contrôleur qui s'assure que tout est bien cadré après une redimension de
* fenêtre.
*/

class ResizingController : public Collector<sf::Event&> {
	private :
		Scene & m_scene;

	public :
		ResizingController(Scene & scene, InputManager & i);

		void collect(sf::Event & e);

		void discard(sf::Event & e);
};

/**
* Controleur qui permet de déplacer la caméra autour de la scène, à la souris.
* (En vérité c'est la scène qui tourne)
*/

class CameraController : public Collector<sf::Event &> {
	protected :
		Scene & m_scene;
		bool m_moving;
		int m_x_anchor;
		int m_y_anchor;
		float m_anchor_x_angle;
		float m_anchor_y_angle;
	public :
		CameraController(Scene & scene, InputManager & i);

		void collect(sf::Event & e);
		void discard(sf::Event & e);
};

#endif
