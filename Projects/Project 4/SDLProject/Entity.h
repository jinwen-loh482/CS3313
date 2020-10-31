#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"

enum EntityType { PLAYER, PLATFORM, ENEMY };
enum AIType { DETECTOR, PATROL, TIMED };
enum AIState { IDLE, ACTIVE };

class Entity {
public:
    Entity();
    void setVertices(float vertices[]);
    void setTexCoords(float texCoords[]);
    void Update(float deltaTime, Entity *player, Entity* objects, int tileCount);
    void Render(ShaderProgram *program);
    
    void AI(Entity *player, float deltaTime);
    void Detector(Entity *player);
    void Timed(Entity *player, float deltaTime);
    
    bool DetectGap(float deltaTime, Entity* objects, int tileCount);
    void ResetCollisionFlags();
    bool CheckCollision(Entity *other);
    bool CheckScreenCollision();
    void CheckCollisionsX(Entity *objects, int objectCount);
    void CheckCollisionsY(Entity *objects, int objectCount);
    
    glm::mat4 modelMatrix;
    float width = 1;
    float height = 1;
    
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 acceleration;
    
    float vertices[12] = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5,
        -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
    float texCoords[12] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0,
        0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };
    
    EntityType entityType;
    AIType aiType;
    AIState aiState;
    float accumulator = 0;
    
    bool collidedTop = false;
    bool collidedBottom = false;
    bool collidedLeft = false;
    bool collidedRight = false;
    
    bool isActive = 1;
    
    GLuint textureID;
};
