#include "PlayerInterface.h"
#include <GL/gl.h>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#define bitSize 0.125
#define MIN_GAME 7200

PlayerInterface::PlayerInterface() {
	lives = 0;
	item = Item::NONE;
}

PlayerInterface::~PlayerInterface() {

}

void PlayerInterface::init(ShaderProgram& program) {
	this->program = program;

	text.loadFromFile("images/Items.png", TEXTURE_PIXEL_FORMAT_RGBA);
	text.setWrapS(GL_CLAMP_TO_EDGE);
	text.setWrapT(GL_CLAMP_TO_EDGE);
	text.setMinFilter(GL_NEAREST);
	text.setMagFilter(GL_NEAREST);

	prepareLivesSprite();
	prepareItemsSprite();


	if (!text2.init("fonts/OpenSans-Regular.ttf"))
		cout << "Could not load font!!!" << endl;

	
}

void PlayerInterface::update(int deltaTime) {


}
void PlayerInterface::render() {

	glm::mat4 modelview;



	glEnable(GL_TEXTURE_2D);
	text.use();

	if (lives >= 1) {
		program.setUniformMatrix4f("modelview", live1Pos);
		program.setUniform2f("texCoordDispl", 3 * bitSize, 2 * bitSize);
		glBindVertexArray(vaoLives);
		glEnableVertexAttribArray(posLocationLive);
		glEnableVertexAttribArray(texCoordLocationLive);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	if (lives >= 2) {
		program.setUniformMatrix4f("modelview", live2Pos);
		program.setUniform2f("texCoordDispl", 3 * bitSize, 2 * bitSize);
		glBindVertexArray(vaoLives);
		glEnableVertexAttribArray(posLocationLive);
		glEnableVertexAttribArray(texCoordLocationLive);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	if (lives >= 3) {
		program.setUniformMatrix4f("modelview", live3Pos);
		program.setUniform2f("texCoordDispl", 3 * bitSize, 2 * bitSize);
		glBindVertexArray(vaoLives);
		glEnableVertexAttribArray(posLocationLive);
		glEnableVertexAttribArray(texCoordLocationLive);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	switch (item)
	{
	case Item::STOP_WATCH:
		program.setUniformMatrix4f("modelview", itemPos);
		program.setUniform2f("texCoordDispl", 3 * bitSize, 0);
		glBindVertexArray(vaoItem);
		glEnableVertexAttribArray(posLocationItem);
		glEnableVertexAttribArray(texCoordLocationItem);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		break;
	case Item::DINAMITE:
	case Item::INVENCIBLE:
	case Item::NONE:
	default:
		break;
	}

	glDisable(GL_TEXTURE_2D);

	text2.render("SCORE:"+ to_string(score), glm::vec2(500, 900), 40, glm::vec4(1, 1, 1, 1));
	text2.render("TIME:" + to_string(time), glm::vec2(900, 900), 40, glm::vec4(1, 1, 1, 1));
}

void PlayerInterface::setLives(int l) {
	if (l >= 0 && l <= 3) {
		lives = l;
	}
}

void PlayerInterface::prepareLivesSprite() {
	float vertices[24] = {
		// Posiciones          // Coordenadas de Textura
		0.0f, 0.0f, 0.0f, 0.0f, // Esquina superior izquierda
		0.0f, 16.f, 0.0f, bitSize, // Esquina inferior izquierda
		16.f, 16.f, bitSize, bitSize, // Esquina inferior derecha

		0.0f, 0.0f, 0.0f, 0.0f, // Esquina inferior izquierda
		16.f, 16.f, bitSize, bitSize, // Esquina superior derecha
		16.f, 0.0f, bitSize, 0.0f // Esquina inferior derecha
	};

	glGenVertexArrays(1, &vaoLives);
	glBindVertexArray(vaoLives);
	glGenBuffers(1, &vboLives);
	glBindBuffer(GL_ARRAY_BUFFER, vboLives);
	glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(float), vertices, GL_STATIC_DRAW);
	
	posLocationLive = program.bindVertexAttribute("position", 2, 4 * sizeof(float), 0);
	texCoordLocationLive = program.bindVertexAttribute("texCoord", 2, 4 * sizeof(float), (void*)(2 * sizeof(float)));


	live1Pos = glm::translate(glm::mat4(1.0f), glm::vec3(16, 8 * 28, 0.f));
	live2Pos = glm::translate(glm::mat4(1.0f), glm::vec3(32, 8 * 28, 0.f));
	live3Pos = glm::translate(glm::mat4(1.0f), glm::vec3(48, 8 * 28, 0.f));
}

void PlayerInterface::setItem(Item::ITEM_TYPE i) {
	item = i;
}

void PlayerInterface::prepareItemsSprite() {
	float vertices[24] = {
		// Posiciones          // Coordenadas de Textura
		0.0f, 0.0f, 0.0f, 0.0f, // Esquina superior izquierda
		0.0f, 16.f, 0.0f, bitSize, // Esquina inferior izquierda
		16.f, 16.f, bitSize, bitSize, // Esquina inferior derecha

		0.0f, 0.0f, 0.0f, 0.0f, // Esquina inferior izquierda
		16.f, 16.f, bitSize, bitSize, // Esquina superior derecha
		16.f, 0.0f, bitSize, 0.0f // Esquina inferior derecha
	};

	glGenVertexArrays(1, &vaoItem);
	glBindVertexArray(vaoItem);
	glGenBuffers(1, &vboItem);
	glBindBuffer(GL_ARRAY_BUFFER, vboItem);
	glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(float), vertices, GL_STATIC_DRAW);

	posLocationItem = program.bindVertexAttribute("position", 2, 4 * sizeof(float), 0);
	texCoordLocationItem = program.bindVertexAttribute("texCoord", 2, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	itemPos = glm::translate(glm::mat4(1.0f), glm::vec3(96, 8 * 28, 0.f));
}
void PlayerInterface::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if (!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if (!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if (!texProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}
	texProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}


void PlayerInterface::setScore(int s){
	score = s;
}


void PlayerInterface::setTime(int t) {
	time = t;
}
