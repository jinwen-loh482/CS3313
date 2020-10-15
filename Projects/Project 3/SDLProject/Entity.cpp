#include "Entity.h"

Entity::Entity() : modelMatrix(1), position(0), velocity(0), acceleration(0) {}

void Entity::setVertices(float vertices[12]) {
    for (int i = 0; i < 12; ++i) this->vertices[i] = vertices[i];
}

void Entity::setTexCoords(float texCoords[12]) {
    for (int i = 0; i < 12; ++i) this->texCoords[i] = texCoords[i];
}

bool Entity::CheckCollision(Entity *other) {
    float xdist = fabs(position.x - other->position.x) - ((width + other->width) / 2.0f);
    float ydist = fabs(position.y - other->position.y) - ((height + other->height) / 2.0f);
    
    if (xdist < 0 && ydist < 0) return true;
    return false;
}

void Entity::CheckCollisionsX(Entity *objects, int objectCount) {
    for (int i = 0; i < objectCount; ++i) {
        Entity *object = &objects[i];
        
        if (CheckCollision(object)) {
            if (velocity.x > 0) {
                collidedRight = true;
            } else if (velocity.x < 0) {
                collidedLeft = true;
            }
            velocity = glm::vec3(0);
        }
    }
}

void Entity::CheckCollisionsY(Entity *objects, int objectCount) {
    for (int i = 0; i < objectCount; ++i) {
        Entity *object = &objects[i];
        
        if (CheckCollision(object)) {
            if (velocity.y > 0) {
                collidedTop = true;
            } else if (velocity.y < 0) {
                collidedBottom = true;
            }
            velocity = glm::vec3(0);        }
    }
}

void Entity::Update(float deltaTime, Entity* objects, int tileCount) {
    velocity += deltaTime * acceleration;
    position += deltaTime * velocity;
    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, position);
    CheckCollisionsX(objects, tileCount);
    CheckCollisionsY(objects, tileCount);
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
