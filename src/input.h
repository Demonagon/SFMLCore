#ifndef INPUT__H
#define INPUT__H

#include <SFML/Window.hpp>
#include <list>
#include <thread>
#include "provider.h"
#include "work.h"

/**
Le travail de gérer les inputs est ici géré par un Worker alloué pour l'occasion.
Cela permet une gestion rapide des inputs, ce qui est nécessaire pour une bonne
interface utilisateur.
**/

class InputManager : public StandardListProvider<sf::Event &> {
	protected :
		bool m_interrupted;
		MutexDataLock m_interrupt_lock;
		sf::Window & m_window;
		DataLock & m_window_lock;
	public :
		InputManager(sf::Window & window, DataLock & window_lock);

		void inputLoop();
		void interruptLoop();
};


#endif
