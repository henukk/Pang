#ifndef _TILE_MAP_INCLUDE
#define _TILE_MAP_INCLUDE


#include <glm/glm.hpp>
#include "Texture.h"
#include "ShaderProgram.h"


// Class Tilemap is capable of loading a tile map from a text file in a very
// simple format (see level01.txt for an example). With this information
// it builds a single VBO that contains all tiles. As a result the render
// method draws the whole map independently of what is visible.


class TileMap
{

private:
	TileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program);

	void paintBackground(ShaderProgram& program);

public:
	// Tile maps can only be created inside an OpenGL context
	static TileMap *createTileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program);

	~TileMap();

	void render() const;
	void free();
	
	int getTileSize() const { return tileSize; }

	bool collisionMoveLeft(const glm::ivec2 &pos, const glm::ivec2 &size) const;
	bool collisionMoveRight(const glm::ivec2 &pos, const glm::ivec2 &size) const;
	bool collisionMoveUp(const glm::ivec2& pos, const glm::ivec2& size) const;
	bool collisionMoveDown(const glm::ivec2& pos, const glm::ivec2& size) const;

	bool bStairs(const glm::ivec2& pos, const glm::ivec2& size) const;

	bool bStairsDown(const glm::ivec2& pos, const glm::ivec2& size) const;

	bool collisionMoveUpPlayer(const glm::ivec2& pos, const glm::ivec2& size) const;
	bool collisionMoveDownPlayer(const glm::ivec2& pos, const glm::ivec2& size) const;

	void getRightTile(const glm::ivec2& pos, const glm::ivec2& size);

	bool floorDown(const glm::ivec2& pos, const glm::ivec2& size) const;
	
	
private:
	bool loadLevel(const string &levelFile);
	void prepareArrays(const glm::vec2 &minCoords, ShaderProgram &program);

private:
	GLuint vao;
	GLuint vbo;
	GLint posLocation, texCoordLocation;
	int nTiles;
	glm::ivec2 position, mapSize, tilesheetSize;
	int tileSize;
	Texture tilesheet;
	glm::vec2 tileTexSize;
	int *map;


	GLuint vaoBackground;
	GLuint vboBackground;
	string backgroundFile;
	Texture background;
	GLint posLocationBackground, texCoordLocationBackground;
};


#endif // _TILE_MAP_INCLUDE


