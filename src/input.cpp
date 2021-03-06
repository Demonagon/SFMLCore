#include "input.h"
#include <iostream>
#include "work.h"


InputManager::InputManager(sf::Window & window, DataLock & window_lock) :
	m_interrupted(false),
	m_interrupt_lock(),
	m_window(window),
	m_window_lock(window_lock)
{}

void
InputManager::inputLoop() {
	sf::Event event;
	m_interrupt_lock.lock();
	while( ! m_interrupted ) {
		m_interrupt_lock.unlock();
		m_window_lock.lock();
			bool is_event = m_window.pollEvent(event);
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
