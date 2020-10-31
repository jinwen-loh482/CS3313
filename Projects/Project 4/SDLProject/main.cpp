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

#define TOTAL_FLOOR 10
#define TOTAL_PLATFORM1 4
#define TOTAL_PLATFORM2 4
#define TOTAL_PLATFORMS TOTAL_FLOOR + TOTAL_PLATFORM1 + TOTAL_PLATFORM2
#define TOTAL_AI 3
#define TOTAL_NPO TOTAL_PLATFORMS + TOTAL_AI

#define GRAVITY -9.81
#define MOVE_SPEED 2
#define AI_MOVE_SPEED 1
#define JUMP_SPEED 6.65


SDL_Window* displayWindow;
bool gameIsRunning = true;
int gameState = NOT_STARTED;

float lastTicks = 0;
float accumulator = 0;

ShaderProgram program;
glm::mat4 viewMatrix, projectionMatrix;
GLuint fontTextureID;
GLuint platformTextureID;
GLuint aiTextureID;
GLuint playerTextureID;

Entity player;
Entity AIs[TOTAL_AI];
Entity platforms[TOTAL_PLATFORMS];
Entity NPO[TOTAL_AI+TOTAL_PLATFORMS];

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
    
    // Player
    playerTextureID = LoadTexture("assets/platformChar_idle.png");
    player.entityType = PLAYER;
    player.textureID = playerTextureID;
    player.position = glm::vec3(-4.5, -2.25, 0);
    player.acceleration = glm::vec3(0, GRAVITY, 0);
    player.Update(0, NULL, NULL, 0);
    
    // Platforms
    glm::vec3 initFloorPosition = {-4.5, -3.25, 0};
    glm::vec3 initPlatform1Position = { 4.5, -1.25, 0};
    glm::vec3 initPlatform2Position = {-4.5, 0.5, 0};
    platformTextureID = LoadTexture("assets/platformPack_tile001.png");
    for (int i = 0; i < TOTAL_PLATFORMS; ++i) {
        NPO[i].entityType = PLATFORM;
        NPO[i].textureID = platformTextureID;
        if (i >= TOTAL_FLOOR + TOTAL_PLATFORM1) {
            NPO[i].position = initPlatform2Position;
            initPlatform2Position.x += 1;
        } else if (i >= TOTAL_FLOOR) {
            NPO[i].position = initPlatform1Position;
            initPlatform1Position.x -= 1;
        }
        else {
            NPO[i].position = initFloorPosition;
            initFloorPosition.x += 1;
        }
        NPO[i].Update(0, NULL, NULL, 0);
    }
    
    // AIs
    aiTextureID = LoadTexture("assets/ctg.png");
    for (int i = TOTAL_PLATFORMS; i < TOTAL_NPO; ++i) {
        switch(i) {
            case(TOTAL_PLATFORMS):
                NPO[i].aiType = DETECTOR;
                NPO[i].position = glm::vec3(3, -2.25, 0);
                break;
            case(TOTAL_PLATFORMS+1):
                NPO[i].aiType = TIMED;
                NPO[i].position = glm::vec3(4.0, -0.25, 0);
                break;
            case(TOTAL_PLATFORMS+2):
                NPO[i].aiType = PATROL;
                NPO[i].position = glm::vec3(-4.5, 1.5, 0);
                NPO[i].aiState = ACTIVE;
                NPO[i].velocity.x = 1;
        }
        NPO[i].entityType = ENEMY;
        NPO[i].aiState = IDLE;
        NPO[i].textureID = aiTextureID;
        NPO[i].acceleration = glm::vec3(0, GRAVITY, 0);
        NPO[i].Update(0, &player, NPO, TOTAL_NPO);
    }
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
                    case SDLK_RETURN:
                        if (gameState==NOT_STARTED) {
                            gameState = IN_PROGRESS;
                        }
                        break;
                }
                break;
        }
    }
    
    const Uint8 *keys = SDL_GetKeyboardState(NULL);
           
    if (gameState == IN_PROGRESS) {
        if (keys[SDL_SCANCODE_A]) {
            player.velocity.x = -MOVE_SPEED;
        }
        else if (keys[SDL_SCANCODE_D]) {
            player.velocity.x = MOVE_SPEED;
        } else player.velocity.x = 0;
        if (keys[SDL_SCANCODE_SPACE]) {
            if (player.collidedBottom) player.velocity.y = JUMP_SPEED;
        }
    }
}

void Update() {
    if (gameState == IN_PROGRESS) {
        float ticks = (float)SDL_GetTicks() / 1000.0f;
        float deltaTime = ticks - lastTicks;
        lastTicks = ticks;
        deltaTime += accumulator;
        if (deltaTime < FIXED_TIMESTEP) {
            accumulator = deltaTime;
            return;
        }
        player.Update(FIXED_TIMESTEP, NULL, NPO, TOTAL_NPO);
        for (int i = TOTAL_PLATFORMS; i < TOTAL_NPO; ++i)
            NPO[i].Update(FIXED_TIMESTEP, &player, NPO, TOTAL_NPO);
        if (!player.isActive)
            gameState = GAME_LOST;
        int activeBots = TOTAL_AI;
        for (int i = TOTAL_PLATFORMS; i < TOTAL_NPO; ++i)
            if (!NPO[i].isActive)
                activeBots -= 1;
        if (activeBots == 0)
            gameState = GAME_WON;
        deltaTime -= FIXED_TIMESTEP;
        accumulator = deltaTime;
    }
}

void Render() {
    glClear(GL_COLOR_BUFFER_BIT);
    for (int i = 0; i < TOTAL_PLATFORMS; ++i)
        NPO[i].Render(&program);
    for (int i = TOTAL_PLATFORMS; i < TOTAL_NPO; ++i)
        NPO[i].Render(&program);
    player.Render(&program);
    if (gameState == NOT_STARTED) {
        DrawText(&program, fontTextureID, "Press ENTER to begin!", 0.5f, -.25f, glm::vec3(-2.5,0,0));
    } else if (gameState == GAME_WON) {
        DrawText(&program, fontTextureID, "You Win!", 0.5f, -.25f, glm::vec3(-1.5,0,0));
    } else if (gameState == GAME_LOST) {
        DrawText(&program, fontTextureID, "You Lose!", 0.5f, -.25f, glm::vec3(-1.5,0,0));
    }
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
