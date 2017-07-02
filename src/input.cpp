#include "input.h"
#include <iostream>
#include "work.h"


InputManager::InputManager(sf::Window & window, DataLock & window_lock) :
	m_interrupted(false),
	m_interrupt_lock(),
	m_window(window),
	m_window_lock(window_lock)
	//m_thread()
{
	//m_thread = std::thread(&InputManager::inputLoop, this);
}

void
InputManager::inputLoop() {
	sf::Event event;
	m_interrupt_lock.lock();
	while( ! m_interrupted ) {
		m_interrupt_lock.unlock();
		m_window_lock.lock();
		//m_window.setActive(true);
		bool is_event = m_window.pollEvent(event);
		//m_window.setActive(false);
		m_window_lock.unlock();
		if(is_event)
			provide(event);
		m_interrupt_lock.lock();
	}
	m_interrupt_lock.unlock();
}

void 
InputManager::interruptLoop() {
	m_interrupt_lock.lock();
	m_interrupted = true;
	m_interrupt_lock.unlock();
}

/*void
InputManager::join() {
	m_thread.join();
}

void
InputManager::terminateAndJoin() {
	interruptLoop();
	m_thread.join();
}*/
