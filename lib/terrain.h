#ifndef TERRAIN__H
#define TERRAIN__H
#include <vector>
#include "metadata.h"
#include <SFML/Graphics.hpp>

class TerrainType : public MetaDataObject {
	protected :
	public :
		TerrainType();
		virtual ~TerrainType();
};

class Terrain {
	protected :
		std::vector< std::vector< double > > m_height_map;
		std::vector< std::vector< TerrainType > > m_terrain_types;
		int m_width;
		int m_height;
	public :
		static const int DEFAULT_WIDTH = 100;
		static const int DEFAULT_HEIGHT = 100;

		Terrain();
		Terrain(int width, int height);
		virtual ~Terrain();

		std::vector< std::vector< double > > & getHeightMap();
		std::vector< std::vector< TerrainType > > & getTerrainTypes();

		int getWidth();
		int getHeight();

	private :
		void init_data();
};

class TerrainGenerator {
		
	public :
		TerrainGenerator() {}
};

class TerrainVisualizer {
	protected :
		Terrain & m_terrain;
		sf::VertexArray m_visualization;
	public :
		TerrainVisualizer(Terrain & terrain);
		virtual ~TerrainVisualizer();

		void generateVisualization(sf::VertexArray & array, int * index,
					  int i, int j, Terrain & terrain);
		void generateVisualization();

		void display();
		void translate(double x, double y, double z, int * screen_x, int * screen_y);
};

#endif
