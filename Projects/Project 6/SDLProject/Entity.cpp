#include "Entity.h"

Entity::Entity()
{
    position = glm::vec3(0);
    movement = glm::vec3(0);
    acceleration = glm::vec3(0);
    speed = 0;
    
    modelMatrix = glm::mat4(1.0f);
}

bool Entity::CheckCollision(Entity *other) {
    if (isActive == false || other->isActive == false) return false;
    
    float xdist = fabs(position.x - other->position.x) - ((width + other->width) / 2.0f);
    float ydist = fabs(position.y - other->position.y) - ((height + other->height) / 2.0f);
    
    if (xdist < 0 && ydist < 0) return true;
    return false;
}

void Entity::CheckCollisionsX(Map *map)
{
    // Probes for tiles
    glm::vec3 left = glm::vec3(position.x - (width / 2), position.y, position.z);
    glm::vec3 right = glm::vec3(position.x + (width / 2), position.y, position.z);

    float penetration_x = 0;
    float penetration_y = 0;
    if (map->IsSolid(left, &penetration_x, &penetration_y) && movement.x < 0) {
        position.x += penetration_x;
        movement.x = 0;
        collidedLeft = true;
    }

    if (map->IsSolid(right, &penetration_x, &penetration_y) && movement.x > 0) {
        position.x -= penetration_x;
        movement.x = 0;
        collidedRight = true;
    }
}

void Entity::CheckCollisionsX(Entity *objects, int objectCount) {
    for (int i = 0; i < objectCount; ++i) {
        Entity *object = &objects[i];
        if (this != object) {
            if (CheckCollision(object)) {
                float xdist = fabs(position.x - object->position.x);
                float penetrationX = fabs(xdist - (height / 2.0f) - object->height / 2.0f);
                if (movement.x > 0) {
                    position.x -= penetrationX;
                    movement.x = 0;
                    collidedRight = true;
                } else if (movement.x < 0) {
                    position.x += penetrationX;
                    movement.x = 0;
                    collidedLeft = true;
                }
                if (entityType == PLAYER && object->entityType == ENEMY)
                isActive = false;
            }
        }
    }
}

void Entity::CheckCollisionsY(Map *map)
{
    // Probes for tiles
    glm::vec3 top = glm::vec3(position.x, position.y + (height / 2), position.z);
    glm::vec3 top_left = glm::vec3(position.x - (width / 2), position.y + (height / 2), position.z);
    glm::vec3 top_right = glm::vec3(position.x + (width / 2), position.y + (height / 2), position.z);

    glm::vec3 bottom = glm::vec3(position.x, position.y - (height / 2), position.z);
    glm::vec3 bottom_left = glm::vec3(position.x - (width / 2), position.y - (height / 2), position.z);
    glm::vec3 bottom_right = glm::vec3(position.x + (width / 2), position.y - (height / 2), position.z);

    float penetration_x = 0;
    float penetration_y = 0;
    if (map->IsSolid(top, &penetration_x, &penetration_y) && movement.y > 0) {
    position.y -= penetration_y;
    movement.y = 0;
    collidedTop = true;
    }
    else if (map->IsSolid(top_left, &penetration_x, &penetration_y) && movement.y > 0) {
        position.y -= penetration_y;
        movement.y = 0;
        collidedTop = true;
    }
    else if (map->IsSolid(top_right, &penetration_x, &penetration_y) && movement.y > 0) {
        position.y -= penetration_y;
        movement.y = 0;
        collidedTop = true;
    }
    if (map->IsSolid(bottom, &penetration_x, &penetration_y) && movement.y < 0) {
        position.y += penetration_y;
        movement.y = 0;
        collidedBottom = true;
    }
    else if (map->IsSolid(bottom_left, &penetration_x, &penetration_y) && movement.y < 0) {
        position.y += penetration_y;
        movement.y = 0;
        collidedBottom = true;
    }
    else if (map->IsSolid(bottom_right, &penetration_x, &penetration_y) && movement.y < 0) {
        position.y += penetration_y;
        movement.y = 0;
        collidedBottom = true;
    }
}

void Entity::CheckCollisionsY(Entity *objects, int objectCount) {
    for (int i = 0; i < objectCount; ++i) {
        Entity *object = &objects[i];
        if (this != object) {
            if (CheckCollision(object)) {
                float ydist = fabs(position.y - object->position.y);
                float penetrationY = fabs(ydist - (height / 2.0f) - object->height / 2.0f);
                if (movement.y > 0) {
                    position.y -= penetrationY;
                    movement.y = 0;
                    collidedTop = true;
                } else if (movement.y < 0) {
                    position.y += penetrationY;
                    movement.y = 0;
                    collidedBottom = true;
                }
                if (entityType == PLAYER && object->entityType == ENEMY)
                    object->isActive = false;
            }
        }
    }
}

bool Entity::DetectGap(float deltaTime, Map *map) {
    Entity ghost = *this;
    ghost.movement += deltaTime * ghost.acceleration;
    
    if (ghost.movement.x > 0)
        ghost.position.x += deltaTime * ghost.movement.x + ghost.width;
    else if (ghost.movement.x < 0)
        ghost.position.x += deltaTime * ghost.movement.x - ghost.width;
    ghost.CheckCollisionsX(map);
    
    ghost.position.y += deltaTime * ghost.movement.y;
    ghost.CheckCollisionsY(map);
    
    return !(ghost.collidedBottom);
}

void Entity::AIWalker() {
    if (movement == glm::vec3(0))
        movement = glm::vec3(-1, 0, 0);
}

void Entity::Detector(Entity *player) {
    switch(aiState) {
        case IDLE:
            if ((glm::distance(position, player->position) < 3)) {
                aiState = ACTIVE;
            } break;
        case ACTIVE:
            if (glm::length(movement) == 0) {
                if (player->position.x > position.x)
                    movement = glm::vec3(1, 0, 0);
                else
                    movement = glm::vec3(-1, 0, 0);
            }
            break;
    }
}

void Entity::AI(Entity *player) {
    switch(aiType) {
        case WALKER:
            AIWalker();
            break;
        case DETECTOR:
            Detector(player);
            break;
    }
}

void Entity::Update(float deltaTime, Entity *player, Entity *objects, int objectCount, Map *map, int scene)
{
    if (!isActive) return;
    
    collidedTop = false;
    collidedBottom = false;
    collidedLeft = false;
    collidedRight = false;
    
    if (entityType == ENEMY) {
        AI(player);
        if (DetectGap(deltaTime, map)) {
            if (scene == 2 || scene == 3)
                movement.x *= -1;
        }
    }
    
    movement += acceleration * deltaTime;
    
    position.x += movement.x * deltaTime; // Move on X
    CheckCollisionsX(map);
    CheckCollisionsX(objects, objectCount); // Fix if needed
    
    position.y +=  movement.y * deltaTime; // Move on Y
    CheckCollisionsY(map);
    CheckCollisionsY(objects, objectCount); // Fix if needed
    
    
    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, position);
}

void Entity::Render(ShaderProgram *program) {
    if (!isActive) return;
    
    program->SetModelMatrix(modelMatrix);
    
    float vertices[]  = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
    float texCoords[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };
    
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program->positionAttribute);
    
    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program->texCoordAttribute);
    
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
}
