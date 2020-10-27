#define GL_SILENCE_DEPRECATION
#include <iostream>
#include <vector>
#include <string>

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

#include "Entity.h"

#define FIXED_TIMESTEP 0.01666666666

#define GAME_LOST 0
#define GAME_WON 1
#define IN_PROGRESS 2
#define NOT_STARTED 3

#define TOTAL_PLATFORMS 10


SDL_Window* displayWindow;
bool gameIsRunning = true;
int gameState = NOT_STARTED;

float lastTicks = 0;
float accumulator = 0;

ShaderProgram program;
glm::mat4 viewMatrix, projectionMatrix;

GLuint fontTextureID;
GLuint platformTextureID;
GLuint playerTextureID;

Entity player;
Entity platforms[TOTAL_PLATFORMS];

GLuint LoadTexture(const char* filePath) {
    int w, h, n;
    unsigned char* image = stbi_load(filePath, &w, &h, &n, STBI_rgb_alpha);
    
    if (image == NULL) {
        std::cout << "Unable to load image. Make sure the path is correct\n";
        assert(false);
    }
    
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
    stbi_image_free(image);
    return textureID;
}

void DrawText(ShaderProgram *program, GLuint fontTextureID, std::string text, float size, float spacing, glm::vec3 position) {
    float width = 1.0f/16.0f;
    float height = 1.0f/16.0f;
    
    std::vector<float> vertices;
    std::vector<float> texCoords;
    
    for (int i = 0; i < text.size(); ++i) {
        int index = (int)text[i];
        float offset = (size+spacing)*i;
        
        float u = (float)(index % 16) / 16.0f;
        float v = (float)(index / 16) / 16.0f;
        vertices.insert(vertices.end(), {
            offset + (-0.5f * size), 0.5f * size,
            offset + (-0.5f * size), -0.5f * size,
            offset + (0.5f * size), 0.5f * size,
            offset + (0.5f * size), -0.5f * size,
            offset + (0.5f * size), 0.5f * size,
            offset + (-0.5f * size), -0.5f * size
        });
        
        texCoords.insert(texCoords.end(), {
            u, v,
            u, v + height,
            u + width, v,
            u + width, v + height,
            u + width, v,
            u, v + height
        });
    }
    
    glm::mat4 modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, position);
    program->SetModelMatrix(modelMatrix);
    
    glUseProgram(program->programID);
    
    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices.data());
    glEnableVertexAttribArray(program->positionAttribute);
    
    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords.data());
    glEnableVertexAttribArray(program->texCoordAttribute);
    
    glBindTexture(GL_TEXTURE_2D, fontTextureID);
    glDrawArrays(GL_TRIANGLES, 0, (int)(text.size() * 6));
    
    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
}

void Initialize() {
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("Project 4!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
    
#ifdef _WINDOWS
    glewInit();
#endif
    
    glViewport(0, 0, 640, 480);
    
    program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");
    
    viewMatrix = glm::mat4(1.0f);
    projectionMatrix = glm::mat4(1.0f);
    
    projectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);
    
    program.SetViewMatrix(viewMatrix);
    program.SetProjectionMatrix(projectionMatrix);
    
    glUseProgram(program.programID);
    
    glClearColor(0, 0, 0, 1);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // Set up game objects
    // Font
    fontTextureID = LoadTexture("assets/font1.png");
    
    // Platforms
    glm::vec3 initPosition = {-4.5, -3.25, 0};
    platformTextureID = LoadTexture("assets/platformPack_tile001.png");
    for (int i = 0; i < TOTAL_PLATFORMS; ++i) {
        platforms[i].entityType = PLATFORM;
        platforms[i].textureID = platformTextureID;
        platforms[i].position = initPosition;
        platforms[i].Update(0, NULL, 0);
        initPosition.x += 1;
    }
    
    // Player
    playerTextureID = LoadTexture("assets/platformChar_idle.png");
    player.entityType = PLAYER;
    player.textureID = playerTextureID;
    player.position = glm::vec3(-4.25, -3.25, 0);
    player.acceleration = glm::vec3(0, -2, 0);
    player.Update(0, NULL, 0);
}

void ProcessInput() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
            case SDL_WINDOWEVENT_CLOSE:
                gameIsRunning = false;
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_SPACE:
                        if (gameState==NOT_STARTED) {
                            gameState = IN_PROGRESS;
                        }
                        break;
                }
                break;
        }
    }
}

void Update() {
    float ticks = (float)SDL_GetTicks() / 1000.0f;
    float deltaTime = ticks - lastTicks;
    lastTicks = ticks;
    deltaTime += accumulator;
    if (deltaTime < FIXED_TIMESTEP) {
        accumulator = deltaTime;
        return;
    }
    player.Update(FIXED_TIMESTEP, platforms, TOTAL_PLATFORMS);
    deltaTime -= FIXED_TIMESTEP;
    accumulator = deltaTime;
}

void Render() {
    glClear(GL_COLOR_BUFFER_BIT);
    for (int i = 0; i < TOTAL_PLATFORMS; ++i)
        platforms[i].Render(&program);
    player.Render(&program);
    SDL_GL_SwapWindow(displayWindow);
}

void Shutdown() { SDL_Quit(); }

int main(int argc, char* argv[]) {
    Initialize();
    while(gameIsRunning) {
        ProcessInput();
        Update();
        Render();
    }
    Shutdown();
    return 0;
}
