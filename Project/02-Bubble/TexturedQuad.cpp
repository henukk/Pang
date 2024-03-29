#include <GL/glew.h>
#include <GL/gl.h>
#include "TexturedQuad.h"




TexturedQuad* TexturedQuad::createTexturedQuad(glm::vec2 geom[2], glm::vec2 texCoords[2], ShaderProgram& program) {
    return new TexturedQuad(geom, texCoords, program);
}

TexturedQuad::TexturedQuad(glm::vec2 geom[2], glm::vec2 texCoords[2], ShaderProgram& program) {
    float vertices[24] = {
        geom[0].x, geom[0].y, texCoords[0].x, texCoords[0].y,
        geom[1].x, geom[0].y, texCoords[1].x, texCoords[0].y,
        geom[1].x, geom[1].y, texCoords[1].x, texCoords[1].y,
        geom[0].x, geom[0].y, texCoords[0].x, texCoords[0].y,
        geom[1].x, geom[1].y, texCoords[1].x, texCoords[1].y,
        geom[0].x, geom[1].y, texCoords[0].x, texCoords[1].y
    };

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    posLocation = program.bindVertexAttribute("position", 2, 4 * sizeof(float), 0);
    texCoordLocation = program.bindVertexAttribute("texCoord", 2, 4 * sizeof(float), (void*)(2 * sizeof(float)));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void TexturedQuad::render(const Texture& tex) const {
    tex.use();
    glBindVertexArray(vao);
    glEnableVertexAttribArray(posLocation);
    glEnableVertexAttribArray(texCoordLocation);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glDisableVertexAttribArray(posLocation);
    glDisableVertexAttribArray(texCoordLocation);
    glBindVertexArray(0);
}

void TexturedQuad::free() const{
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
}

