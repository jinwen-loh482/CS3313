#pragma once
#define GL_SILENCE_DEPRECATION
#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#define GL_GLEXT_PROTOTYPES 1
#include <vector>
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"
#include "Util.h"
#include "Entity.h"
#include "Map.h"


enum GameControl {NOT_STARTED, IN_PROGRESS, GAME_WON, GAME_LOST };

struct GameState {
    Map *map;
    Entity *player;
    Entity *enemies;
    int nextScene;
    int sceneNumber;
    int livesLeft;
    GameControl control = NOT_STARTED;
};

class Scene {
public:
    GameState state;
    GLuint fontTextureID;
    virtual void Initialize(int lives) = 0;
    virtual void Update(float deltaTime) = 0;
    virtual void Render(ShaderProgram *program) = 0;
};
