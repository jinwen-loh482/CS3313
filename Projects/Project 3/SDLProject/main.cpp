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

#include "Entity.h"

#define FIXED_TIMESTEP 0.01666666666
#define WINNING_TILE 7
#define BOTTOM_TILES 10
#define SIDE_TILES 7
#define PLATFORMS 2
#define TOTAL_TILES 26

#define GAME_LOST 0
#define GAME_WON 1
#define IN_PROGRESS 2
#define NOT_STARTED 3


SDL_Window* displayWindow;
bool gameIsRunning = true;
int gameState = NOT_STARTED;

float lastTicks = 0;
float accumulator = 0;

ShaderProgram program;
glm::mat4 viewMatrix, projectionMatrix;

Entity player;
Entity tiles[TOTAL_TILES];

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

void Initialize() {
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("Project 3!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
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
    // Player
    player.position = glm::vec3(0, 3, 0);
    player.width = 0.5;
    player.height = 0.5;
    float playerVertices[] = { -0.25, -0.25, 0.25, -0.25, 0.25, 0.25,
        -0.25, -0.25, 0.25, 0.25, -0.25, 0.25 };
    player.setVertices(playerVertices);
    player.textureID = LoadTexture("assets/playerShip3_red.png");
    player.Update(0, NULL, 0);
    
    // Tiles
    float x_tile_pos = -5;
    float y_tile_pos = -3.75;
    for (int i = 0; i < TOTAL_TILES; ++i) {
        if (i == WINNING_TILE)
            tiles[i].textureID = LoadTexture("assets/platformPack_tile008.png");
        else
            tiles[i].textureID = LoadTexture("assets/platformPack_tile016.png");
        
        tiles[i].position = glm::vec3(x_tile_pos+0.5, y_tile_pos+0.5, 0);
        tiles[i].Update(0, NULL, 0);
        if (i < BOTTOM_TILES) {
            x_tile_pos += 1;
            if (i == BOTTOM_TILES-1) {
                x_tile_pos = -5;
                y_tile_pos = -2.75;
            }
        } else if (i < BOTTOM_TILES+(2*SIDE_TILES)) {
            y_tile_pos += 1;
            if (i == BOTTOM_TILES+SIDE_TILES-1) {
                x_tile_pos = 4;
                y_tile_pos = -2.75;
            } else if (i == BOTTOM_TILES+(2*SIDE_TILES)-1) {
                x_tile_pos = -2;
                y_tile_pos = -1;
            }
        } else x_tile_pos += 1;
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
                    case SDLK_SPACE:
                        if (gameState==NOT_STARTED) {
                            gameState = IN_PROGRESS;
                            player.acceleration.y = -0.5;
                        }
                        break;
                }
                break;
        }
    }
    
    const Uint8 *keys = SDL_GetKeyboardState(NULL);
    
    if (gameState == IN_PROGRESS) {
        if (keys[SDL_SCANCODE_A]) player.acceleration.x = -1;
        else if (keys[SDL_SCANCODE_D]) player.acceleration.x = 1;
        else player.acceleration.x = 0;
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
    if (player.collidedBottom && tiles[WINNING_TILE].collidedTop) gameState = GAME_WON;
    else if (player.collidedTop || player.collidedBottom || player.collidedLeft || player.collidedRight) {
        gameState = GAME_LOST;
        player.acceleration = glm::vec3(0);
    }
    
    while (deltaTime >= FIXED_TIMESTEP) {
        player.Update(FIXED_TIMESTEP, tiles, TOTAL_TILES);
        tiles[WINNING_TILE].Update(FIXED_TIMESTEP, &player, 1);
        deltaTime -= FIXED_TIMESTEP;
    }
    accumulator = deltaTime;
}

void Render() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    for (int i=0; i < TOTAL_TILES; ++i) { tiles[i].Render(&program); }
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
