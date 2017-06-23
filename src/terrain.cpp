#include "terrain.h"
#include <iostream>

/** TerrainType **/

TerrainType::TerrainType() {
	getMetaData().addClass<TerrainType>();
}

TerrainType::~TerrainType() {}

/** Terrain **/

Terrain::Terrain() {
	m_width = DEFAULT_WIDTH;
	m_height = DEFAULT_HEIGHT;
	
	init_data();
}

Terrain::Terrain(int width, int height) {
	m_width = width;
	m_height = height;
	
	init_data();
}

Terrain::~Terrain() {}

std::vector< std::vector< double > > &
Terrain::getHeightMap() {
	return m_height_map;
}

std::vector< std::vector< TerrainType > > &
Terrain::getTerrainTypes() {
	return m_terrain_types;
}

int
Terrain::getWidth() {
	return m_width;
}

int
Terrain::getHeight() {
	return m_height;
}

void
Terrain::init_data() {
	m_height_map = std::vector< std::vector< double > >();
	for(int i = 0; i < m_width; i++) {
		m_height_map.push_back(std::vector< double >() );
		for(int j = 0; j < m_height; j++)
			m_height_map.back().push_back(j + i);
	}

	m_terrain_types = std::vector< std::vector< TerrainType > >();
	for(int i = 0; i < m_width - 1; i++) {
		m_terrain_types.push_back(std::vector< TerrainType >() );
		for(int j = 0; j < m_height - 1; j++)
			m_terrain_types.back().push_back( TerrainType() );
	}
}

//TerrainVisualizer

TerrainVisualizer::TerrainVisualizer(Terrain & terrain) : m_terrain(terrain) {
	generateVisualization();
}
TerrainVisualizer::~TerrainVisualizer() {}

void
TerrainVisualizer::generateVisualization(sf::VertexArray & array, int * index,
					  int i, int j, Terrain & terrain) {
	int x = 0, y = 0;
	translate(i, j, terrain.getHeightMap()[i][j], &x, &y);

	m_visualization[*index].color = sf::Color::White;
	m_visualization[*index].position = sf::Vector2f(x, y);

	m_visualization[(*index) + 2].color = sf::Color::White;
	m_visualization[(*index) + 2].position = sf::Vector2f(x, y);

	(*index)++;
	translate(i + 1, j, terrain.getHeightMap()[i + 1][j], &x, &y);

	m_visualization[*index].color = sf::Color::White;
	m_visualization[*index].position = sf::Vector2f(x, y);

	(*index) += 2;
	translate(i, j + 1, terrain.getHeightMap()[i][j + 1], &x, &y);

	m_visualization[*index].color = sf::Color::White;
	m_visualization[*index].position = sf::Vector2f(x, y);

	(*index)++;
}

void
TerrainVisualizer::generateVisualization()
{
	int lines_count = 4 * m_terrain.getWidth() * m_terrain.getHeight()
		+ 2 * m_terrain.getWidth() + 2 * m_terrain.getHeight() - 2;

	m_visualization = sf::VertexArray(sf::Lines, lines_count);


	int index = 0;

	for(int i = 0; i < m_terrain.getWidth() - 1; i++)
		for(int j = 0; j < m_terrain.getHeight() - 1; j++)
			generateVisualization(m_visualization, &index,
					  i, j, m_terrain);

	
}

void
TerrainVisualizer::display() {
    sf::RenderWindow window(sf::VideoMode(1500, 800), "SFML works!");

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(m_visualization);
        window.display();
    }
}

void
TerrainVisualizer::translate(double x, double y, double z, int * screen_x, int * screen_y) {
	
		// s_y = offset - ( x * cos_angle_horizontal_camera + y * sin_angle_horizontal_camera ) * angle_vertical_camera
	//s_y = y_offset - ( x + y ) * angle_vertical_camera
	//					+ z * ( max_angle - angle_vertical_camera )
	//s_x = x_offset - ( y - x ) * angle_vertical_camera
	double s_x = 330 - (x + y) * 2.0 - z;
	double s_y = 90 - (y - x);
	*screen_x = s_x * 5;
	*screen_y = s_y * 5;
}
