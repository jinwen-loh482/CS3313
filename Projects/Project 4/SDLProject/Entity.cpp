#include "Entity.h"

Entity::Entity() : modelMatrix(1), position(0), velocity(0), acceleration(0) {}

void Entity::setVertices(float vertices[12]) {
    for (int i = 0; i < 12; ++i) this->vertices[i] = vertices[i];
}

void Entity::setTexCoords(float texCoords[12]) {
    for (int i = 0; i < 12; ++i) this->texCoords[i] = texCoords[i];
}

void Entity::ResetCollisionFlags() {
    collidedBottom = false;
    collidedTop = false;
    collidedRight = false;
    collidedLeft = true;
}

bool Entity::CheckCollision(Entity *other) {
    // Entity collision
    float xdist = fabs(position.x - other->position.x) - ((width + other->width) / 2.0f);
    float ydist = fabs(position.y - other->position.y) - ((height + other->height) / 2.0f);
    
    if (xdist < 0 && ydist < 0) return true;
    return false;
}

bool Entity::CheckScreenCollision() {
    float left = position.x-width/2;
    float right = position.x+width/2;
    float up = position.y+height/2;
    float down = position.y-height/2;
    if (left < -5 || right > 5 || up > 3.75 || down < -3.75) return true;
    return false;
}

void Entity::CheckCollisionsX(Entity *objects, int objectCount) {
    for (int i = 0; i < objectCount; ++i) {
        Entity *object = &objects[i];
        
        if (CheckCollision(object)) {
            float xdist = fabs(position.x - object->position.x);
            float penetrationX = fabs(xdist - (width / 2.0f) - object->width / 2.0f);
            if (velocity.x > 0) {
                position.x -= penetrationX;
                collidedRight = true;
            } else if (velocity.x < 0) {
                position.x += penetrationX;
                collidedLeft = true;
            }
            velocity.x = 0;
        }
    }
    if (CheckScreenCollision()) {
        float left = position.x-width/2;
        float right = position.x+width/2;
        if (left < -5) {
            position.x += fabs(-5-left);
        }
        else if (right > 5) {
            position.x -= fabs(5-right);
        }
        velocity.x = 0;
    }
}

void Entity::CheckCollisionsY(Entity *objects, int objectCount) {
    for (int i = 0; i < objectCount; ++i) {
        Entity *object = &objects[i];
        
        if (CheckCollision(object)) {
            float ydist = fabs(position.y - object->position.y);
            float penetrationY = fabs(ydist - (height / 2.0f) - object->height / 2.0f);
            if (velocity.y > 0) {
                position.y -= penetrationY;
                collidedTop = true;
            } else if (velocity.y < 0) {
                position.y += penetrationY;
                collidedBottom = true;
            }
            velocity.y = 0;
        }
    }
    if (CheckScreenCollision()) {
        float up = position.y+height/2;
        float down = position.y-height/2;
        if (up > 3.75) {
            position.y -= fabs(3.75-up);
            collidedTop = true;
        }
        else if (down < -3.75) {
            position.y -= fabs(-3.75-down);
            collidedBottom = true;
        }
        velocity.y = 0;
    }
}

void Entity::Detector(Entity *player) {
    switch(aiState) {
        case IDLE:
            if (player->position.x < position.x)
                velocity = glm::vec3(-1, 0, 0);
            else
                velocity = glm::vec3(1, 0, 0);
            break;
        case ACTIVE:
            if (player->position.x < position.x)
                velocity = glm::vec3(-1, 0, 0);
            else
                velocity = glm::vec3(1, 0, 0);
            break;
    }
}

void Entity::AI(Entity* player) {
    switch(aiType) {
        case DETECTOR: {
            Detector(player);
            break;
        }
        case PATROL: {
            break;
        }
        case TIMED: {
            break;
        }
    }
}

void Entity::Update(float deltaTime, Entity *player, Entity* objects, int tileCount) {
    if (!isActive) return;
    ResetCollisionFlags();
    
    if (entityType == ENEMY) {
        AI(player);
    }
    
    velocity += deltaTime * acceleration;
    
    position.y += deltaTime * velocity.y;
    CheckCollisionsY(objects, tileCount);
    
    position.x += deltaTime * velocity.x;
    CheckCollisionsX(objects, tileCount);
    
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
