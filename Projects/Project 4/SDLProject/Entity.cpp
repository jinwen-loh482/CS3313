#include "Entity.h"

Entity::Entity() : modelMatrix(1), position(0), velocity(0), acceleration(0) {}

void Entity::setVertices(float vertices[12]) {
    for (int i = 0; i < 12; ++i) this->vertices[i] = vertices[i];
}

void Entity::setTexCoords(float texCoords[12]) {
    for (int i = 0; i < 12; ++i) this->texCoords[i] = texCoords[i];
}

bool Entity::CheckCollision(Entity *other) {
    // Entity collision
    float xdist = fabs(position.x - other->position.x) - ((width + other->width) / 2.0f);
    float ydist = fabs(position.y - other->position.y) - ((height + other->height) / 2.0f);
    
    if (xdist < 0 && ydist < 0) {
        // Penetration correction
        // travelling right
        if (velocity.x > 0)
            position.x -= fabs(xdist);
        if (velocity.x < 0)
            position.x += fabs(xdist);
        // travelling up
        if (velocity.y > 0)
            position.y -= fabs(ydist);
        if (velocity.y < 0)
            position.y += fabs(ydist);
        return true;
    }
    return false;
}

bool Entity::CheckScreenCollision() {
    float left = position.x-width/2;
    float right = position.x+width/2;
    float up = position.y+height/2;
    float down = position.y-height/2;
    
    if (left < -5) {
        position.x += fabs(-5-left);
        return true;
    }
    if (right > 5) {
        position.x -= fabs(5-right);
        return true;
    }
    if (up > 3.75) {
        position.y -= fabs(3.75-up);
        return true;
    }
    if (down < -3.75) {
        position.y += fabs(-3.75-down);
        return true;
    }
    return false;
}

void Entity::CheckCollisionsX(Entity *objects, int objectCount) {
    if (CheckScreenCollision()) {
        if (velocity.x > 0) {
            collidedRight = true;
        } else if (velocity.x < 0) {
            collidedLeft = true;
        }
        velocity.x = 0;
    }
    for (int i = 0; i < objectCount; ++i) {
        Entity *object = &objects[i];
        
        if (CheckCollision(object)) {
            if (velocity.x > 0) {
                collidedRight = true;
            } else if (velocity.x < 0) {
                collidedLeft = true;
            }
            velocity.x = 0;
        }
    }
}

void Entity::CheckCollisionsY(Entity *objects, int objectCount) {
    if (CheckScreenCollision()) {
        if (velocity.y > 0) {
            collidedTop = true;
        } else if (velocity.y < 0) {
            collidedBottom = true;
        }
        velocity.y = 0;
    }
    for (int i = 0; i < objectCount; ++i) {
        Entity *object = &objects[i];
        
        if (CheckCollision(object)) {
            if (velocity.y > 0) {
                collidedTop = true;
            } else if (velocity.y < 0) {
                collidedBottom = true;
            }
            velocity.y = 0;
        }
    }
}

void Entity::Update(float deltaTime, Entity* objects, int tileCount) {
    velocity += deltaTime * acceleration;
    position += deltaTime * velocity;
    CheckCollisionsX(objects, tileCount);
    CheckCollisionsY(objects, tileCount);
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
