#include "Entity.h"

Entity::Entity() : modelMatrix(1), position(0), velocity(0), acceleration(0) {}

void Entity::setVertices(float vertices[12]) {
    for (int i = 0; i < 12; ++i) this->vertices[i] = vertices[i];
}

void Entity::setTexCoords(float texCoords[12]) {
    for (int i = 0; i < 12; ++i) this->texCoords[i] = texCoords[i];
}

void Entity::Update(float deltaTime) {
    velocity += deltaTime * acceleration;
    position += deltaTime * velocity;
    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, position);
}

void Entity::Render(ShaderProgram *program) {
    program->SetModelMatrix(modelMatrix);
    
    glBindTexture(GL_TEXTURE_2D, textureID);
        
    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program->positionAttribute);
    
    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program->texCoordAttribute);
    
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
}
