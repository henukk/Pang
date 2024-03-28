#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "TileMap.h"


using namespace std;


TileMap* TileMap::createTileMap(const string& levelFile, const glm::vec2& minCoords, ShaderProgram& program)
{
	TileMap* map = new TileMap(levelFile, minCoords, program);

	return map;
}


TileMap::TileMap(const string& levelFile, const glm::vec2& minCoords, ShaderProgram& program)
{
	loadLevel(levelFile);
	prepareArrays(minCoords, program);
	paintBackground(program);
}

TileMap::~TileMap()
{
	if (map != NULL)
		delete map;
}

void TileMap::paintBackground(ShaderProgram& program) {
	GLuint backgroundTexture;
	int imageWidth, imageHeight;
	unsigned char* imageData;

	background.loadFromFile(backgroundFile, TEXTURE_PIXEL_FORMAT_RGBA);
	background.setWrapS(GL_CLAMP_TO_EDGE);
	background.setWrapT(GL_CLAMP_TO_EDGE);
	background.setMinFilter(GL_NEAREST);
	background.setMagFilter(GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int backgroundHeigth = (mapSize.y - 4) * tileSize;
	int bakcgroundWidth = mapSize.x * tileSize;

	float vertices[24] = {
		// Posiciones          // Coordenadas de Textura
		0.0f, 0.0f, 0.0f, 0.0f, // Esquina superior izquierda
		0.0f, backgroundHeigth, 0.0f, 1.0f, // Esquina inferior izquierda
		bakcgroundWidth, backgroundHeigth, 1.0f, 1.0f, // Esquina inferior derecha

		0.0f, 0.0f, 0.0f, 0.0f, // Esquina inferior izquierda
		bakcgroundWidth, backgroundHeigth, 1.0f, 1.0f, // Esquina superior derecha
		bakcgroundWidth, 0.0f, 1.0f, 0.0f // Esquina inferior derecha
	};

	glGenVertexArrays(1, &vaoBackground);
	glBindVertexArray(vaoBackground);
	glGenBuffers(1, &vboBackground);
	glBindBuffer(GL_ARRAY_BUFFER, vboBackground);
	glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(float), vertices, GL_STATIC_DRAW);

	posLocationBackground = program.bindVertexAttribute("position", 2, 4 * sizeof(float), 0);
	texCoordLocationBackground = program.bindVertexAttribute("texCoord", 2, 4 * sizeof(float), (void*)(2 * sizeof(float)));
}


void TileMap::render() const
{
	glEnable(GL_TEXTURE_2D);

	background.use();
	glBindVertexArray(vaoBackground);
	glEnableVertexAttribArray(posLocationBackground);
	glEnableVertexAttribArray(texCoordLocationBackground);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	tilesheet.use();
	glBindVertexArray(vao);
	glEnableVertexAttribArray(posLocation);
	glEnableVertexAttribArray(texCoordLocation);
	glDrawArrays(GL_TRIANGLES, 0, 6 * nTiles);
	glDisable(GL_TEXTURE_2D);
}

void TileMap::free()
{
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &vboBackground);
}

bool TileMap::loadLevel(const string& levelFile)
{
	ifstream fin;
	string line, tilesheetFile;
	stringstream sstream;
	char tile;

	fin.open(levelFile.c_str());
	if (!fin.is_open())
		return false;
	getline(fin, line);
	if (line.compare(0, 7, "TILEMAP") != 0)
		return false;
	getline(fin, line);
	sstream.str(line);
	sstream >> mapSize.x >> mapSize.y;

	getline(fin, line);
	sstream.str(line);
	sstream >> tileSize;

	getline(fin, line);
	sstream.str(line);
	sstream >> backgroundFile;


	getline(fin, line);
	sstream.str(line);
	sstream >> tilesheetFile;
	tilesheet.loadFromFile(tilesheetFile, TEXTURE_PIXEL_FORMAT_RGBA);
	tilesheet.setWrapS(GL_CLAMP_TO_EDGE);
	tilesheet.setWrapT(GL_CLAMP_TO_EDGE);
	tilesheet.setMinFilter(GL_NEAREST);
	tilesheet.setMagFilter(GL_NEAREST);
	getline(fin, line);
	sstream.str(line);
	sstream >> tilesheetSize.x >> tilesheetSize.y;
	tileTexSize = glm::vec2(1.f / tilesheetSize.x, 1.f / tilesheetSize.y);


	map = new int[mapSize.x * mapSize.y];
	for (int j = 0; j < mapSize.y; j++) {
		std::getline(fin, line);

		std::stringstream ss(line);
		std::string token;
		for (int i = 0; i < mapSize.x; i++) {
			std::getline(ss, token, ','); // Extrae un número separado por coma
			int n = std::stoi(token) + 1;
				map[j * mapSize.x + i] = n; // Convierte la cadena a número
		}
	#ifndef _WIN32
			fin.get(tile);
	#endif
	}
	fin.close();

	for (int i = 0; i < mapSize.x; i++)
	{
		map[i] = -1;
		map[(mapSize.y - 5) * mapSize.x + i] = -1;
	}

	for (int j = 0; j < mapSize.y-5; j++)
	{
		map[j * mapSize.x] = -1;
		map[j * mapSize.x + mapSize.x-1] = -1;
	}

	return true;
}

void TileMap::prepareArrays(const glm::vec2& minCoords, ShaderProgram& program)
{
	int tile;
	int blockSize = 8;
	glm::vec2 posTile, texCoordTile[2], halfTexel;
	vector<float> vertices;

	nTiles = 0;
	halfTexel = glm::vec2(0.5f / tilesheet.width(), 0.5f / tilesheet.height());
	for (int j = 0; j < mapSize.y; j++)
	{
		for (int i = 0; i < mapSize.x; i++)
		{
			tile = map[j * mapSize.x + i];
			if (tile > 0)
			{
				// Non-empty tile
				nTiles++;
				posTile = glm::vec2(minCoords.x + i * tileSize, minCoords.y + j * tileSize);
				texCoordTile[0] = glm::vec2(float((tile - 1) % tilesheetSize.x) / tilesheetSize.x, float((tile - 1) / tilesheetSize.x) / tilesheetSize.y);
				texCoordTile[1] = texCoordTile[0] + tileTexSize;
				//texCoordTile[0] += halfTexel;
				texCoordTile[1] -= halfTexel;
				// First triangle
				vertices.push_back(posTile.x); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y + blockSize);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[1].y);
				// Second triangle
				vertices.push_back(posTile.x); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y + blockSize);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[1].y);
				vertices.push_back(posTile.x); vertices.push_back(posTile.y + blockSize);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[1].y);
			}
		}
	}

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 24 * nTiles * sizeof(float), &vertices[0], GL_STATIC_DRAW);
	posLocation = program.bindVertexAttribute("position", 2, 4 * sizeof(float), 0);
	texCoordLocation = program.bindVertexAttribute("texCoord", 2, 4 * sizeof(float), (void*)(2 * sizeof(float)));
}

// Collision tests for axis aligned bounding boxes.
// Method collisionMoveDown also corrects Y coordinate if the box is
// already intersecting a tile below.

bool TileMap::collisionMoveLeft(const glm::ivec2& pos, const glm::ivec2& size) const
{
	int x, y0, y1;

	x = pos.x / tileSize;
	y0 = pos.y / tileSize;
	y1 = (pos.y + size.y - 1) / tileSize;
	for (int y = y0; y <= y1; y++)
	{


		int coliderbox = map[y * mapSize.x + x];
		if (coliderbox != 0 && coliderbox != 13 && coliderbox != 11 && coliderbox != 12 && coliderbox != 45 && coliderbox != 43 && coliderbox != 44)
			return true;
	}

	return false;
}

bool TileMap::collisionMoveRight(const glm::ivec2& pos, const glm::ivec2& size) const
{
	int x, y0, y1;

	x = (pos.x + size.x - 1) / tileSize;
	y0 = pos.y / tileSize;
	y1 = (pos.y + size.y - 1) / tileSize;
	for (int y = y0; y <= y1; y++)
	{
		int coliderbox = map[y * mapSize.x + x];
		
		if (coliderbox != 0 && coliderbox != 13 && coliderbox != 11 && coliderbox != 12 && coliderbox != 45 && coliderbox != 43 && coliderbox != 44){
			return true;
		}
	}

	return false;
}

bool TileMap::collisionMoveUp(const glm::ivec2& pos, const glm::ivec2& size) const
{
	int x0, x1, y;

	// Calcula las posiciones de los tiles en los extremos izquierdo y derecho del objeto
	x0 = pos.x / tileSize;
	x1 = (pos.x + size.x - 1) / tileSize;

	// Calcula la posici�n del tile justo arriba del objeto
	y = pos.y / tileSize; // Restamos 1 para obtener el tile encima del objeto

	// Itera sobre los tiles en el rango horizontal del objeto
	for (int x = x0; x <= x1; x++)
	{
		// Verifica si el tile actual es s�lido. Tambi�n comprueba que 'y' no es negativo para evitar �ndices fuera de rango.
		if (y >= 0 && map[y * mapSize.x + x] != 0)
			return true; // Retorna true si hay colisi�n
	}

	return false; // No hay colisi�n
}


bool TileMap::collisionMoveDown(const glm::ivec2& pos, const glm::ivec2& size) const
{
	int x0, x1, y;

	// Calcula las posiciones de los tiles en los extremos izquierdo y derecho del objeto
	x0 = pos.x / tileSize;
	x1 = (pos.x + size.x - 1) / tileSize;

	// Calcula la posici�n del tile justo debajo del objeto
	y = (pos.y + size.y - 1) / tileSize;

	// Itera sobre los tiles en el rango horizontal del objeto
	for (int x = x0; x <= x1; x++)
	{
		// Verifica si el tile actual es s�lido
		
		if (map[y * mapSize.x + x] != 0)
			return true; // Retorna true si hay colisi�n
	}

	return false; // No hay colisi�n
}

bool TileMap::bStairs(const glm::ivec2& pos, const glm::ivec2& size) const {
    // Calculamos las coordenadas del tile para las esquinas superior izquierda e inferior derecha del objeto
    int xLeft = pos.x / tileSize;
    int xRight = (pos.x + size.x - 1) / tileSize;
    int yTop = pos.y / tileSize;
    int yBottom = (pos.y + size.y - 1) / tileSize;

    // Verificamos cada tile que el objeto ocupa para ver si alguno es una escalera
    for (int x = xLeft; x <= xRight; ++x) {
        for (int y = yTop; y <= yBottom; ++y) {
            int tileType = map[y * mapSize.x + x];
            // Si el tile es uno de los tipos que representan escaleras, retornamos true
            if (tileType == 10 || tileType == 11 || tileType == 12 ||
                tileType == 45 || tileType == 43 || tileType == 44) {
                return true; // Coincide con un tile de escalera
            }
        }
    }

    return false; // No coincide con un tile de escalera
}

bool TileMap::bStairsDown(const glm::ivec2& pos, const glm::ivec2& size) const {
	// Calculamos las coordenadas del tile para las esquinas superior izquierda e inferior derecha del objeto
	int xLeft = pos.x / tileSize;
	int xRight = (pos.x + size.x - 1) / tileSize;
	int yTop = pos.y / tileSize;
	int yBottom = (pos.y + size.y - 1) / tileSize + 1; // Incluimos los tiles debajo del objeto

	// Asegurarnos de que yBottom no excede el límite del mapa
	yBottom = std::min(yBottom, mapSize.y - 1);

	// Verificamos cada tile que el objeto ocupa y los tiles debajo de él para ver si alguno es una escalera
	for (int x = xLeft; x <= xRight; ++x) {
		for (int y = yTop; y <= yBottom; ++y) {
			int tileType = map[y * mapSize.x + x];
			// Si el tile es uno de los tipos que representan escaleras, retornamos true
			if (tileType == 10 || tileType == 11 || tileType == 12 ||
				tileType == 45 || tileType == 43 || tileType == 44) {
				return true; // Coincide con un tile de escalera
			}
		}
	}

	return false; // No coincide con un tile de escalera
}


bool TileMap::collisionMoveUpPlayer(const glm::ivec2& pos, const glm::ivec2& size) const
{
	int x0, x1, y;

	// Calcula las posiciones de los tiles en los extremos izquierdo y derecho del objeto
	x0 = pos.x / tileSize;
	x1 = (pos.x + size.x - 1) / tileSize;

	// Calcula la posici�n del tile justo arriba del objeto
	y = pos.y / tileSize; // Restamos 1 para obtener el tile encima del objeto

	// Itera sobre los tiles en el rango horizontal del objeto
	for (int x = x0; x <= x1; x++)
	{
		// Verifica si el tile actual es s�lido. Tambi�n comprueba que 'y' no es negativo para evitar �ndices fuera de rango.
		int tileType = map[y * mapSize.x + x];
		if (y >= 0 && tileType != 0 && tileType != 13 && tileType != 11 && tileType != 12 &&
			tileType != 45 && tileType != 43 && tileType != 44)
			return true; // Retorna true si hay colisi�n
	}

	return false; // No hay colisi�n
}

bool TileMap::collisionMoveDownPlayer(const glm::ivec2& pos, const glm::ivec2& size) const
{
	int x0, x1, y;

	// Calcula las posiciones de los tiles en los extremos izquierdo y derecho del objeto
	x0 = pos.x / tileSize;
	x1 = (pos.x + size.x - 1) / tileSize;

	// Calcula la posici�n del tile justo debajo del objeto
	y = (pos.y + size.y - 1) / tileSize;

	// Itera sobre los tiles en el rango horizontal del objeto
	for (int x = x0; x <= x1; x++)
	{
		// Verifica si el tile actual es s�lido
		int tileType = map[y * mapSize.x + x];
		if (tileType != 0 && tileType != 13 && tileType != 11 && tileType != 12 &&
			tileType != 45 && tileType != 43 && tileType != 44)

			return true; // Retorna true si hay colisi�n
	}

	return false; // No hay colisi�n
}

void TileMap::getRightTile(const glm::ivec2& pos, const glm::ivec2& size) {

	int x0, x1, y;

	// Calcula las posiciones de los tiles en los extremos izquierdo y derecho del objeto
	x0 = pos.x / tileSize;
	x1 = (pos.x + size.x - 1) / tileSize;

	// Calcula la posici�n del tile justo debajo del objeto
	y = (pos.y + size.y - 1) / tileSize;

	// Itera sobre los tiles en el rango horizontal del objeto
	for (int x = x0; x <= x1; x++)
	{
		// Verifica si el tile actual es s�lido
		cout << map[y * mapSize.x + x] << endl;

	}
}


bool TileMap::floorDown(const glm::ivec2& pos, const glm::ivec2& size) const
{
	int x0, x1, y;

	// Calcula las posiciones de los tiles en los extremos izquierdo y derecho del objeto
	x0 = pos.x / tileSize;
	x1 = (pos.x + size.x - 1) / tileSize;

	// Calcula la posici�n del tile justo debajo del objeto
	y = (pos.y + size.y - 1) / tileSize;

	// Itera sobre los tiles en el rango horizontal del objeto
	for (int x = x0; x <= x1; x++)
	{
		// Verifica si el tile actual es s�lido
		int tileType = map[y * mapSize.x + x];
		if (tileType == 0)

			return true; // Retorna true si hay colisi�n
	}

	return false; // No hay colisi�n
}







