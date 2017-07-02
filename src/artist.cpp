#include "artist.h"
#include <iostream>

Artist::Artist(int width, int height) :
	m_context(24, 8, 4, 3, 3),
    m_window(sf::VideoMode(width, height, 32), "Isotriangle", 7U, m_context),
	m_window_lock(),
	m_thread(),
	m_planned_tick_per_second(60),
	m_interrupted_flag(true),
	m_reg(),
	m_scene(m_window, &m_reg, width, height) {
}

void
Artist::setInputManager(InputManager & i) {
	m_scene.setInputManager(i);
}

void
Artist::paint() {
	m_window_lock.lock();
		m_window.setActive(true);
		m_scene.placeScene();
		m_window.display();
		m_window.setActive(false);
	m_window_lock.unlock();
}

void
Artist::loop() {
	while( ! m_interrupted_flag ) {
		std::chrono::duration<double> tick_min_duration
			(1.0 / m_planned_tick_per_second);

		std::chrono::high_resolution_clock::time_point computation_start
			= std::chrono::high_resolution_clock::now();

		paint();

		if( m_interrupted_flag ) return;

		std::chrono::high_resolution_clock::time_point computation_end
			= std::chrono::high_resolution_clock::now();

		std::chrono::duration<double> computation_time = 
			computation_end - computation_start;

		std::this_thread::sleep_for( tick_min_duration - computation_time );
	}
}

void
Artist::start() {
	m_interrupted_flag = false;
	m_window.setActive(false);
	m_thread = std::thread(& Artist::loop, this);
}

void
Artist::stop() {
	if( m_interrupted_flag ) return;
	m_interrupted_flag = true;
	m_thread.join();
}

Scene &
Artist::getScene() {
	return m_scene;
}

PrintingRegister &
Artist::getPrintingRegister() {
	return m_reg;
}

sf::Window &
Artist::getWindow() {
	return m_window;
}

DataLock &
Artist::getWindowLock() {
	return m_window_lock;
}

void
Artist::close() {
	m_window_lock.lock();
		m_window.close();
	m_window_lock.unlock();
}
