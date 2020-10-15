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

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

class Entity {
public:
    Entity();
    void setVertices(float vertices[]);
    void setTexCoords(float texCoords[]);
    void Update(float deltaTime);
    void Render(ShaderProgram *program);
    
    glm::mat4 modelMatrix;
    
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 acceleration;
    
    float vertices[12] = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5,
        -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
    float texCoords[12] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0,
        0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };
    
    GLuint textureID;
};
