#pragma once
#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1
#include <SDL.h>
#include <SDL_opengl.h>
#include <vector>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"
#include "Map.h"
//#include "projectile.h"

enum EntityType { PLAYER, PLATFORM, ENEMY, PROJECTILE };

enum AIType { SHOOTER, DETECTOR, OSCILLATOR, GUIDED };
enum AIState { IDLE, ACTIVE };


class Entity {
public:
    EntityType entityType;
    AIType aiType;
    AIState aiState;
    glm::vec3 position;
    glm::vec3 movement;
    glm::vec3 acceleration;
    
    float width = 1;
    float height = 1;
    
//    bool jump = false;
//    float jumpPower = 0;

    
    float speed;
    
    GLuint textureID;
//    GLuint projectileID;
    
    glm::mat4 modelMatrix;
    
    bool isActive = true;
    
    bool collidedTop = false;
    bool collidedBottom = false;
    bool collidedLeft = false;
    bool collidedRight = false;
    
    Entity *projectile = nullptr;
//    float projectile_speed = 5;
    
    Entity();
    
    bool CheckCollision(Entity *other);
    void CheckCollisionsX(Entity *objects, int objectCount);
    void CheckCollisionsY(Entity *objects, int objectCount);
    void CheckCollisionsX(Map *map);
    void CheckCollisionsY(Map *map);
    bool collided();
//    bool DetectGap(float deltaTime, Map *map);
    void Update(float deltaTime, Entity *player, Entity *entities, int entityCount, Map *map, int scene);
    void Render(ShaderProgram *program);

    
    void AI(Entity *player);
//    void AIWalker();
    void Detector(Entity *player);
    void Shooter(Entity *player);
    void Guided(Entity *player);
    void Oscillator();
};
