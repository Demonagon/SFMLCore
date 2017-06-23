#ifndef SIMPLE_ARTIST__H
#define SIMPLE_ARTIST__H
#include <SFML/Graphics.hpp>
#include "simple_shapes.h"
#include "collector.h"
#include "work.h"
#include <map>
#include <vector>
#include <thread>
#include <atomic>

/**
Classe qui permet un affichage rapide et simple. Utilise des rectangles colorés
incarnés par la classe SimpleEntity.
**/

class SimpleArtist : public SimpleRectanglePainter,
					 Collector<SimpleRectangleChangeEvent> {
	protected :
		sf::RenderWindow m_window;
		sf::VertexArray m_rectangles;
		std::map<SimpleRectangle *, int> m_index_map;
		std::vector<SimpleRectangle *> m_rectangles_references;
		MutexDataLock m_rectangle_lock;
		MutexDataLock m_window_lock;
		std::thread m_thread;
		int m_planned_tick_per_second;
		std::atomic<bool> m_interrupted_flag;
	public :
		SimpleArtist(int width, int height, sf::Color backround_color);

		void paint();

		void loop();

		void start();

		void stop();

		// Presuppose que le rectangle est déjà dans le vertexarray
		void collect(SimpleRectangleChangeEvent object);
		void discard(SimpleRectangleChangeEvent object);

		// Ajoute le rectangle s'il est nouveau, fait une mise à jour sinon
		void collect(SimpleRectangle & object);

		// Enlève le rectangle de la liste, s'il est dedans.
		void discard(SimpleRectangle & object);

		sf::Window & getWindow();
		DataLock & getWindowLock();

		void close();
};

#endif
