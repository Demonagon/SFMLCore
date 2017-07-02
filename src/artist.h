#ifndef ARTIST__H
#define ARTIST__H
#include <SFML/Graphics.hpp>
#include "work.h"
#include <atomic>
#include <thread>
#include "scene.h"

class Artist {
	protected :
		sf::ContextSettings m_context;
		sf::RenderWindow m_window;
		MutexDataLock m_window_lock;
		
		std::thread m_thread;
		int m_planned_tick_per_second;
		std::atomic<bool> m_interrupted_flag;

		PrintingRegister m_reg;
		Scene m_scene;

	public :
		Artist(int width, int height);

		void paint();

		void loop();

		void start();

		void stop();

		Scene & getScene();
		PrintingRegister & getPrintingRegister();

		sf::Window & getWindow();
		DataLock & getWindowLock();

		void close();
};

#endif
