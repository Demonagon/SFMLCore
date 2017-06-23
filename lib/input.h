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



/*class KeyInputProvider :
	public AbstractEventProvider<KeyEvent> {};
class MouseButtonInputProvider :
	public AbstractEventProvider<MouseButtonEvent> {};
class MouseMoveInputProvider :
	public AbstractEventProvider<MouseMoveEvent> {};
class MouseWheelInputProvider :
	public AbstractEventProvider<MouseWheelEvent> {};
class MouseWheelScrollInputProvider :
	public AbstractEventProvider<MouseWheelScrollEvent> {};
class SizeInputProvider :
	public AbstractEventProvider<SizeEvent> {};*/

class InputManager : public StandardListProvider<sf::Event &> {
	protected :
		bool m_interrupted;
		MutexDataLock m_interrupt_lock;
		sf::Window & m_window;
		DataLock & m_window_lock;
//		std::thread m_thread;
	public :
		InputManager(sf::Window & window, DataLock & window_lock);

		void inputLoop();
		void interruptLoop();
/*		void join();
		void terminateAndJoin();*/
};


#endif
