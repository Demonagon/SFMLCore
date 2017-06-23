#include "simple_artist.h"
#include <map>
#include <iostream>

SimpleArtist::SimpleArtist(int width, int height, sf::Color backround_color) :
	m_window(sf::VideoMode(width, height), "Game"),
	m_rectangles(sf::Quads, 4),
	m_index_map(),
	m_rectangles_references(),
	m_rectangle_lock(),
	m_window_lock(),
	m_thread(),
	m_planned_tick_per_second(60),
	m_interrupted_flag(false) {

	m_rectangle_lock.lock();
		m_rectangles[0].position = sf::Vector2f(0, 0);
		m_rectangles[1].position = sf::Vector2f(width, 0);
		m_rectangles[2].position = sf::Vector2f(width, height);
		m_rectangles[3].position = sf::Vector2f(0, height);

		for(int k = 0; k < 4; k++)
			m_rectangles[k].color = backround_color;
	m_rectangle_lock.unlock();
}

void
SimpleArtist::paint() {
	/*while (m_window.isOpen())
	{
	   // Event processing
	   sf::Event event;
	   while (m_window.pollEvent(event))
	   {
		   // Request for closing the window
		   if (event.type == sf::Event::Closed)
		       m_window.close();
	   }
	   // Clear the whole window before rendering a new frame
	   m_window.clear();

	   m_rectangle_lock.lock();
	   		m_window.draw(m_rectangles);
	   m_rectangle_lock.unlock();

	   // End the current frame and display its contents on screen
	   m_window.display();
	}*/

	m_window_lock.lock();
		m_window.clear();

		m_rectangle_lock.lock();
			m_window.draw(m_rectangles);
		m_rectangle_lock.unlock();

		   // End the current frame and display its contents on screen
		m_window.display();

		m_window.setActive(false);
	m_window_lock.unlock();
}

void
SimpleArtist::loop() {
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
SimpleArtist::start() {
	m_interrupted_flag = false;
	m_thread = std::thread(& SimpleArtist::loop, this);
}

void
SimpleArtist::stop() {
	m_interrupted_flag = true;
	m_thread.join();
}


void 
SimpleArtist::collect(SimpleRectangleChangeEvent o)
{
	int index = m_index_map[&o.rectangle];
	// A

	m_rectangle_lock.lock();
		m_rectangles[index + 0].position
			= sf::Vector2f(o.new_x - o.new_width /2,
						   o.new_y - o.new_height/2);
		// B
		m_rectangles[index + 1].position
			= sf::Vector2f(o.new_x + o.new_width /2,
						   o.new_y - o.new_height/2);
		// C
		m_rectangles[index + 2].position
			= sf::Vector2f(o.new_x + o.new_width /2,
						   o.new_y + o.new_height/2);
		// D
		m_rectangles[index + 3].position
			= sf::Vector2f(o.new_x - o.new_width /2,
						   o.new_y + o.new_height/2);

		for(int k = 0; k < 4; k++)
			m_rectangles[index + k].color = o.new_color;
	m_rectangle_lock.unlock();
}

void 
SimpleArtist::discard(SimpleRectangleChangeEvent object)
{
	//On fait rien, la flemme.
}

void 
SimpleArtist::collect(SimpleRectangle & object)
{
	m_rectangle_lock.lock();
		std::map<SimpleRectangle *, int>::iterator object_iterator
			= m_index_map.find(&object);
		if( object_iterator == m_index_map.end() ) {
			int new_index = m_rectangles.getVertexCount();
			m_index_map[&object] = new_index;
			m_rectangles_references.push_back(&object);
			m_rectangles.resize(new_index + 4);
			object.addCollector(*this);
		}
	m_rectangle_lock.unlock();

	collect( object.generateChangeEvent() );
}

void 
SimpleArtist::discard(SimpleRectangle & object)
{
	m_rectangle_lock.lock();
		std::map<SimpleRectangle *, int>::iterator object_iterator
			= m_index_map.find(&object);
		if( object_iterator == m_index_map.end() ) return;

		int object_index = object_iterator->second;
		int last_index = m_rectangles.getVertexCount() - 4;

		if( object_index != last_index ) {

			SimpleRectangle * last_object = m_rectangles_references[last_index / 4];
			m_index_map[last_object] = object_index;
			m_rectangles_references[object_index / 4] = last_object;
			object.removeCollector(*this);

			for(int k = 0; k < 4; k++)
				m_rectangles[object_index + k] = m_rectangles[last_index + k];
		}

		m_index_map.erase(object_iterator);
		m_rectangles_references.pop_back();
		m_rectangles.resize( m_rectangles.getVertexCount() - 4 );
	m_rectangle_lock.unlock();
}

sf::Window &
SimpleArtist::getWindow() {
	return m_window;
}

DataLock &
SimpleArtist::getWindowLock() {
	return m_window_lock;
}

void
SimpleArtist::close() {
	m_window_lock.lock();
		m_window.close();
	m_window_lock.unlock();
}
