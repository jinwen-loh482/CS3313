#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1
#include <cstdlib>
#include <iostream>
#include <ctime>
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"

SDL_Window* displayWindow;
bool gameIsRunning = true;
bool gameStarted = false;
bool gameEnded = false;

ShaderProgram program;
glm::mat4 viewMatrix, ballMatrix, p1Matrix, p2Matrix, projectionMatrix;

// starting positions
glm::vec3 ball_position = glm::vec3(0,0,0);
glm::vec3 p1_position = glm::vec3(-4.75,0,0);
glm::vec3 p2_position = glm::vec3(4.75,0,0);

// Don't go anywhere (yet)
glm::vec3 ball_movement = glm::vec3(0,0,0);
glm::vec3 p1_movement = glm::vec3(0,0,0);
glm::vec3 p2_movement = glm::vec3(0);

float ball_speed = 3.0f;
float p1_speed = 2.5f;
float p2_speed = 2.5f;

void Initialize() {
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("Project 2 - Pong!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
    
#ifdef _WINDOWS
    glewInit();
#endif
    
    glViewport(0, 0, 640, 480);
    
    program.Load("shaders/vertex.glsl", "shaders/fragment.glsl");
    
    viewMatrix = glm::mat4(1.0f);
    ballMatrix = glm::mat4(1.0f);
    p1Matrix = glm::mat4(1.0f);
    p2Matrix = glm::mat4(1.0f);
    /* Scaling matrix 2x size */
    // modelMatrix = glm::scale(modelMatrix, glm::vec3(2.0f, 2.0f, 1.0f));
    /* 180deg rotation */
    // modelMatrix = glm::rotate(modelMatrix, 3.14f, glm::vec3(0.0f, 0.0f, 1.0f));
    /* Translate 3 to right and 2 to top */
    // modelMatrix = glm::translate(modelMatrix, glm::vec3(3.0f, 2.0f, 0.0f));
    projectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);
    
    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);
    program.SetColor(1.0f, 1.0f, 1.0f, 1.0f);
    
    glUseProgram(program.programID);
    
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
}

void ProcessInput() {
    SDL_Event event;
    p1_movement = glm::vec3(0);
    p2_movement = glm::vec3(0);
    
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
            case SDL_WINDOWEVENT_CLOSE:
                gameIsRunning = false;
                break;
                
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_SPACE:
                        if (!gameStarted) {
                            std::srand(std::time(nullptr));
                            float x_val = 1;
                            float y_val = (std::rand() / RAND_MAX) * 1.75 + .25;
                            if (std::rand() % 2 == 0) x_val *= -1;
                            if (std::rand() % 2 == 1) y_val *= -1;
                            ball_movement.x = x_val;
                            ball_movement.y = y_val;
                            if (glm::length(ball_movement) != 1.0)
                                ball_movement = glm::normalize(ball_movement);
                            gameStarted = true;
                        }
                        break;
                }
                break; //SDL_KEYDOWN
        }
    }
    
    const Uint8 *keys = SDL_GetKeyboardState(NULL);

    if (keys[SDL_SCANCODE_A]) {
        if (p1_position.y+1.25 >= 3.75) p1_movement.y = 0;
        else p1_movement.y = 1.0f;
    } else if (keys[SDL_SCANCODE_D]) {
        if (p1_position.y-1.25 <= -3.75) p1_movement.y = 0;
        else p1_movement.y = -1.0f;
    }
    if (keys[SDL_SCANCODE_LEFT]) {
        if (p2_position.y+1.25 >= 3.75) p2_movement.y = 0;
        else p2_movement.y = 1.0f;
    } else if (keys[SDL_SCANCODE_RIGHT]) {
        if (p2_position.y-1.25 <= -3.75) p2_movement.y = 0;
        else p2_movement.y = -1.0f;
    }
}

float lastTicks;
void Update() {
    if (!gameEnded) {
        float ticks = (float)SDL_GetTicks() / 1000.f;
        float deltaTime = ticks - lastTicks;
        lastTicks = ticks;
        
        // Top-bottom collision
        if (ball_position.y+0.25 >= 3.75 || ball_position.y-0.25 <= -3.75)
            ball_movement.y *= -1;
        
        // End cond
        if (ball_position.x+0.25 >= 5 || ball_position.x-0.25 <= -5)
            gameEnded = true;
        
        float p1_x_diff = fabs(p1_position.x - ball_position.x);
        float p1_y_diff = fabs(p1_position.y - ball_position.y);
        float p1_x_distance = p1_x_diff - (0.5+0.5)/2;
        float p1_y_distance = p1_y_diff - (2.50+0.5)/2;
        
        float p2_x_diff = fabs(p2_position.x - ball_position.x);
        float p2_y_diff = fabs(p2_position.y - ball_position.y);
        float p2_x_distance = p2_x_diff - (0.5+0.5)/2;
        float p2_y_distance = p2_y_diff - (2.50+0.5)/2;
        
        if ((p1_x_distance <= 0 && p1_y_distance <= 0) || (p2_x_distance <= 0 && p2_y_distance <= 0))
            ball_movement.x *= -1;
        
        ball_position += ball_movement * ball_speed * deltaTime;
        ballMatrix = glm::mat4(1.0f);
        ballMatrix = glm::translate(ballMatrix, ball_position);
        
        p1_position += p1_movement * p1_speed * deltaTime;
        p1Matrix = glm::mat4(1.0f);
        p1Matrix = glm::translate(p1Matrix, p1_position);

        p2_position += p2_movement * p2_speed * deltaTime;
        p2Matrix = glm::mat4(1.0f);
        p2Matrix = glm::translate(p2Matrix, p2_position);
    }
}

void Render() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    program.SetModelMatrix(ballMatrix);
    float ballVertices[] = { -0.25, -0.25, 0.25, -0.25, 0.25, 0.25,
                        -0.25, -0.25, 0.25, 0.25, -0.25, 0.25 };
    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, ballVertices);
    glEnableVertexAttribArray(program.positionAttribute);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glDisableVertexAttribArray(program.positionAttribute);
    
    program.SetModelMatrix(p1Matrix);
    //float p1Vertices[] = { -5, -1.25, -4.5, -1.25, -4.5, 1.25,
    //                    -5, -1.25, -5, 1.25, -4.5, 1.25 };
    float p1Vertices[] = { -0.25, -1.25, 0.25, -1.25, 0.25, 1.25, -0.25, -1.25, 0.25, 1.25, -0.25, 1.25 };
    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, p1Vertices);
    glEnableVertexAttribArray(program.positionAttribute);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glDisableVertexAttribArray(program.positionAttribute);
    
    program.SetModelMatrix(p2Matrix);
//    float p2Vertices[] = { 4.5, -1.25, 5, -1.25, 5, 1.25,
//                        4.5, -1.25, 5, 1.25, 4.5, 1.25 };
    float p2Vertices[] = { -0.25, -1.25, 0.25, -1.25, 0.25, 1.25, -0.25, -1.25, 0.25, 1.25, -0.25, 1.25 };
    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, p2Vertices);
    glEnableVertexAttribArray(program.positionAttribute);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glDisableVertexAttribArray(program.positionAttribute);
//
    SDL_GL_SwapWindow(displayWindow);
}

void Shutdown() {
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    Initialize();
    
    while (gameIsRunning) {
        ProcessInput();
        Update();
        Render();
    }
    
    Shutdown();
    return 0;
}
