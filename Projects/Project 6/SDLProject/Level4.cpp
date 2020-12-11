#include "Level4.h"
#define LEVEL4_WIDTH 50
#define LEVEL4_HEIGHT 8

#define LEVEL4_ENEMY_COUNT 11

#define WALL 10

/*
 10, 10, 10, 10, 10, 10, 10, 10, 10, 10,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
 10, 10, 10, 10, 10, 10, 10, 10, 10, 10,
 
 10, 10, 10, 10, 10, 10, 10, 10, 10, 10,
 10,  0,  0,  0,  0,  0,  0,  0,  0,  0,
 10,  0,  0,  0,  0,  0,  0,  0,  0,  0,
 10,  0,  0,  0,  0,  0,  0,  0,  0,  0,
 10,  0,  0,  0,  0,  0,  0,  0,  0,  0,
 10,  0,  0,  0,  0,  0,  0,  0,  0,  0,
 10,  0,  0,  0,  0,  0,  0,  0,  0,  0,
 10, 10, 10, 10, 10, 10, 10, 10, 10, 10,
 */

unsigned int Level4_data[] =
{

    10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10,
    10,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 0,  0,  10,  0,  0,  0,  0,  0,  0,  0, 0,  10, 10, 10,  0,  0,  0,  0,  0,  0, 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    10,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 0,  0,  10,  0,  0,  0,  0,  0,  0,  0, 0,   0, 10, 10,  0,  0,  0,  0,  0,  0, 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    10,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 0,  0,  10,  0,  0,  0,  0, 10, 10, 10, 0,   0,  0, 10,  0,  0,  0,  0,  10,  0, 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    10,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 0,  0,   0,  0,  0,  0, 10, 10, 10, 10, 10,  0,  0,  0,  0,  0,  0,  0,  10,  0, 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    10,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 0,  0,   0,  0,  0, 10, 10, 10, 10, 10, 10, 10,  0,  0,  0,  0,  0,  0,  0,  0, 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    10,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 0,  0,   0, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10,  0,  0,  0,  0,  0,  0,  0, 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10,10,  10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10

};

void Level4::Initialize(int lives) {
    state.livesLeft = lives;
    state.shotsLeft = 2;
    state.sceneNumber = 3;
    state.nextScene = -1;
    state.control = IN_PROGRESS;
    GLuint mapTextureID = Util::LoadTexture("Assets/Textures/monochrome_tilemap.png");
    fontTextureID = Util::LoadTexture("Assets/Textures/font1.png");
    state.map = new Map(LEVEL4_WIDTH, LEVEL4_HEIGHT, Level4_data, mapTextureID, 1.0f, 20, 20);
    // Move over all of the player and enemy code from initialization.
    // Initialize Player
    state.player = new Entity();
    state.player->entityType = PLAYER;
    state.player->position = glm::vec3(5, -5, 0);
    state.player->movement = glm::vec3(1.2, 0, 0);
    state.player->acceleration = glm::vec3(0, 0, 0);
    state.player->speed = 3.0f;
    state.player->textureID = Util::LoadTexture("Assets/Textures/player.png");
    state.player->height = 0.8f;
    state.player->width = 0.8f;
    
    // Initialize projective
    state.player->projectile = new Entity();
    state.player->projectile->entityType = PROJECTILE;
    state.player->projectile->isActive = false;
//    state.player->projectile->position = glm::vec3(5, -5, 0);
    state.player->projectile->textureID = Util::LoadTexture("Assets/Textures/projectile.png");
    state.player->projectile->width = 0.8f;
    state.player->projectile->height = 0.8f;
    
    
    state.enemies = new Entity[LEVEL4_ENEMY_COUNT];
    GLuint enemyTextureID = Util::LoadTexture("Assets/Textures/enemy.png");
//
    state.enemies[0].entityType = ENEMY;
    state.enemies[0].textureID = enemyTextureID;
    state.enemies[0].position = glm::vec3(50, -1.25, 0);
    state.enemies[0].speed = 1;
    state.enemies[0].aiType = SHOOTER;
    state.enemies[0].aiState = IDLE;
    state.enemies[0].isActive = true;
    state.enemies[0].projectile = new Entity();
    state.enemies[0].projectile->entityType = PROJECTILE;
    state.enemies[0].projectile->isActive = false;
    state.enemies[0].projectile->movement = glm::vec3(-3, 0, 0);
//    state.enemies[1].projectile->position = state.enemies[1].position-glm::vec3(1,0,0);
//    state.enemies[1].projectile->movement = glm::vec3(-2, 0, 0);
    state.enemies[0].projectile->textureID = Util::LoadTexture("Assets/Textures/projectile.png");
    state.enemies[0].projectile->width = 0.8f;
    state.enemies[0].projectile->height = 0.8f;
    
    state.enemies[1].entityType = ENEMY;
    state.enemies[1].textureID = enemyTextureID;
    state.enemies[1].position = glm::vec3(50, -2.5, 0);
    state.enemies[1].speed = 1;
    state.enemies[1].aiType = SHOOTER;
    state.enemies[1].aiState = IDLE;
    state.enemies[1].isActive = true;
    state.enemies[1].projectile = new Entity();
    state.enemies[1].projectile->entityType = PROJECTILE;
    state.enemies[1].projectile->isActive = false;
    state.enemies[1].projectile->movement = glm::vec3(-3, 0, 0);
//    state.enemies[1].projectile->position = state.enemies[1].position-glm::vec3(1,0,0);
//    state.enemies[1].projectile->movement = glm::vec3(-2, 0, 0);
    state.enemies[1].projectile->textureID = Util::LoadTexture("Assets/Textures/projectile.png");
    state.enemies[1].projectile->width = 0.8f;
    state.enemies[1].projectile->height = 0.8f;
    
    state.enemies[2].entityType = ENEMY;
    state.enemies[2].textureID = enemyTextureID;
    state.enemies[2].position = glm::vec3(50, -3.75, 0);
    state.enemies[2].speed = 1;
    state.enemies[2].aiType = SHOOTER;
    state.enemies[2].aiState = IDLE;
    state.enemies[2].isActive = true;
    state.enemies[2].projectile = new Entity();
    state.enemies[2].projectile->entityType = PROJECTILE;
    state.enemies[2].projectile->isActive = false;
    state.enemies[2].projectile->movement = glm::vec3(-3, 0, 0);
//    state.enemies[1].projectile->position = state.enemies[1].position-glm::vec3(1,0,0);
//    state.enemies[1].projectile->movement = glm::vec3(-2, 0, 0);
    state.enemies[2].projectile->textureID = Util::LoadTexture("Assets/Textures/projectile.png");
    state.enemies[2].projectile->width = 0.8f;
    state.enemies[2].projectile->height = 0.8f;
    
    state.enemies[3].entityType = ENEMY;
    state.enemies[3].textureID = enemyTextureID;
    state.enemies[3].position = glm::vec3(50, -5.0, 0);
    state.enemies[3].speed = 1;
    state.enemies[3].aiType = SHOOTER;
    state.enemies[3].aiState = IDLE;
    state.enemies[3].isActive = true;
    state.enemies[3].projectile = new Entity();
    state.enemies[3].projectile->entityType = PROJECTILE;
    state.enemies[3].projectile->isActive = false;
    state.enemies[3].projectile->movement = glm::vec3(-3, 0, 0);
//    state.enemies[1].projectile->position = state.enemies[1].position-glm::vec3(1,0,0);
//    state.enemies[1].projectile->movement = glm::vec3(-2, 0, 0);
    state.enemies[3].projectile->textureID = Util::LoadTexture("Assets/Textures/projectile.png");
    state.enemies[3].projectile->width = 0.8f;
    state.enemies[3].projectile->height = 0.8f;
    
    state.enemies[4].entityType = ENEMY;
    state.enemies[4].textureID = enemyTextureID;
    state.enemies[4].position = glm::vec3(10, -3.5, 0);
    state.enemies[4].movement = glm::vec3(-1, 1, 0);
    state.enemies[4].speed = 1;
    state.enemies[4].aiType = OSCILLATOR;
    state.enemies[4].aiState = ACTIVE;
    state.enemies[4].isActive = true;
    state.enemies[4].projectile = new Entity();
    state.enemies[4].projectile->entityType = PROJECTILE;
    state.enemies[4].projectile->isActive = false;
//    state.enemies[0].projectile->position = state.enemies[0].position-glm::vec3(1,0,0);
//    state.enemies[0].projectile->movement = glm::vec3(-1, 0, 0);
    state.enemies[4].projectile->textureID = Util::LoadTexture("Assets/Textures/projectile.png");
    state.enemies[4].projectile->width = 0.8f;
    state.enemies[4].projectile->height = 0.8f;
    
    state.enemies[5].entityType = ENEMY;
    state.enemies[5].textureID = enemyTextureID;
    state.enemies[5].position = glm::vec3(12, -3.5, 0);
    state.enemies[5].movement = glm::vec3(-1, -1, 0);
    state.enemies[5].speed = 1;
    state.enemies[5].aiType = OSCILLATOR;
    state.enemies[5].aiState = ACTIVE;
    state.enemies[5].isActive = true;
    state.enemies[5].projectile = new Entity();
    state.enemies[5].projectile->entityType = PROJECTILE;
    state.enemies[5].projectile->isActive = false;
//    state.enemies[0].projectile->position = state.enemies[0].position-glm::vec3(1,0,0);
//    state.enemies[0].projectile->movement = glm::vec3(-1, 0, 0);
    state.enemies[5].projectile->textureID = Util::LoadTexture("Assets/Textures/projectile.png");
    state.enemies[5].projectile->width = 0.8f;
    state.enemies[5].projectile->height = 0.8f;

    state.enemies[6].entityType = ENEMY;
    state.enemies[6].textureID = enemyTextureID;
    state.enemies[6].position = glm::vec3(14, -3.5, 0);
    state.enemies[6].movement = glm::vec3(-1, 1, 0);
    state.enemies[6].speed = 1;
    state.enemies[6].aiType = OSCILLATOR;
    state.enemies[6].aiState = ACTIVE;
    state.enemies[6].isActive = true;
    state.enemies[6].projectile = new Entity();
    state.enemies[6].projectile->entityType = PROJECTILE;
    state.enemies[6].projectile->isActive = false;
//    state.enemies[0].projectile->position = state.enemies[0].position-glm::vec3(1,0,0);
//    state.enemies[0].projectile->movement = glm::vec3(-1, 0, 0);
    state.enemies[6].projectile->textureID = Util::LoadTexture("Assets/Textures/projectile.png");
    state.enemies[6].projectile->width = 0.8f;
    state.enemies[6].projectile->height = 0.8f;
    
    state.enemies[7].entityType = ENEMY;
    state.enemies[7].textureID = enemyTextureID;
    state.enemies[7].position = glm::vec3(16, -3.5, 0);
    state.enemies[7].movement = glm::vec3(-1, -1, 0);
    state.enemies[7].speed = 1;
    state.enemies[7].aiType = OSCILLATOR;
    state.enemies[7].aiState = ACTIVE;
    state.enemies[7].isActive = true;
    state.enemies[7].projectile = new Entity();
    state.enemies[7].projectile->entityType = PROJECTILE;
    state.enemies[7].projectile->isActive = false;
//    state.enemies[0].projectile->position = state.enemies[0].position-glm::vec3(1,0,0);
//    state.enemies[0].projectile->movement = glm::vec3(-1, 0, 0);
    state.enemies[7].projectile->textureID = Util::LoadTexture("Assets/Textures/projectile.png");
    state.enemies[7].projectile->width = 0.8f;
    state.enemies[7].projectile->height = 0.8f;
    
    state.enemies[8].entityType = ENEMY;
    state.enemies[8].textureID = enemyTextureID;
    state.enemies[8].position = glm::vec3(18, -3.5, 0);
    state.enemies[8].movement = glm::vec3(-1, 1, 0);
    state.enemies[8].speed = 1;
    state.enemies[8].aiType = OSCILLATOR;
    state.enemies[8].aiState = ACTIVE;
    state.enemies[8].isActive = true;
    state.enemies[8].projectile = new Entity();
    state.enemies[8].projectile->entityType = PROJECTILE;
    state.enemies[8].projectile->isActive = false;
//    state.enemies[0].projectile->position = state.enemies[0].position-glm::vec3(1,0,0);
//    state.enemies[0].projectile->movement = glm::vec3(-1, 0, 0);
    state.enemies[8].projectile->textureID = Util::LoadTexture("Assets/Textures/projectile.png");
    state.enemies[8].projectile->width = 0.8f;
    state.enemies[8].projectile->height = 0.8f;
    
    state.enemies[9].entityType = ENEMY;
    state.enemies[9].textureID = enemyTextureID;
    state.enemies[9].position = glm::vec3(20, -3.5, 0);
    state.enemies[9].movement = glm::vec3(-1, -1, 0);
    state.enemies[9].speed = 1;
    state.enemies[9].aiType = OSCILLATOR;
    state.enemies[9].aiState = ACTIVE;
    state.enemies[9].isActive = true;
    state.enemies[9].projectile = new Entity();
    state.enemies[9].projectile->entityType = PROJECTILE;
    state.enemies[9].projectile->isActive = false;
//    state.enemies[0].projectile->position = state.enemies[0].position-glm::vec3(1,0,0);
//    state.enemies[0].projectile->movement = glm::vec3(-1, 0, 0);
    state.enemies[9].projectile->textureID = Util::LoadTexture("Assets/Textures/projectile.png");
    state.enemies[9].projectile->width = 0.8f;
    state.enemies[9].projectile->height = 0.8f;
    
    state.enemies[10].entityType = ENEMY;
    state.enemies[10].textureID = enemyTextureID;
    state.enemies[10].position = glm::vec3(22, -3.5, 0);
    state.enemies[10].movement = glm::vec3(-1, 1, 0);
    state.enemies[10].speed = 1;
    state.enemies[10].aiType = OSCILLATOR;
    state.enemies[10].aiState = ACTIVE;
    state.enemies[10].isActive = true;
    state.enemies[10].projectile = new Entity();
    state.enemies[10].projectile->entityType = PROJECTILE;
    state.enemies[10].projectile->isActive = false;
//    state.enemies[0].projectile->position = state.enemies[0].position-glm::vec3(1,0,0);
//    state.enemies[0].projectile->movement = glm::vec3(-1, 0, 0);
    state.enemies[10].projectile->textureID = Util::LoadTexture("Assets/Textures/projectile.png");
    state.enemies[10].projectile->width = 0.8f;
    state.enemies[10].projectile->height = 0.8f;
    
    
}
void Level4::Update(float deltaTime) {
    if (state.control == IN_PROGRESS) {
        state.player->Update(deltaTime, state.player, state.enemies, LEVEL4_ENEMY_COUNT, state.map, state.sceneNumber);
        for (int i = 0; i < LEVEL4_ENEMY_COUNT; ++i) {
            Entity* enemy = &state.enemies[i];
            enemy->Update(deltaTime, enemy, state.player, 1, state.map, state.sceneNumber);
        }
        
        // Update player projectile
        state.player->projectile->Update(deltaTime, state.player->projectile, state.enemies, LEVEL4_ENEMY_COUNT, state.map, state.sceneNumber);
        for (int i = 0; i < LEVEL4_ENEMY_COUNT; ++i) {
            Entity* enemy = &state.enemies[i];
            enemy->projectile->Update(deltaTime, enemy->projectile, state.player, 1, state.map, state.sceneNumber);
        }
        if (state.player->position.x > 50) {
            state.control = GAME_WON;
//            state.nextScene = 2;
            state.nextScene = -1;
            return;
        }
        if (!state.player->isActive) {
            state.control = GAME_LOST;
            if (state.livesLeft > 0) {
                state.livesLeft--;
                state.control = IN_PROGRESS;
                Initialize(state.livesLeft);
            }
        }
    }
}
void Level4::Render(ShaderProgram *program) {
    state.map->Render(program);
    state.player->Render(program);
    state.player->projectile->Render(program);
    
//    glm::vec3 reminder = glm::vec3(6, -3.5, 0);
//    glm::vec3 reminder_1 = glm::vec3(6, -3.75, 0);
//    glm::vec3 tutorial_shoot_loc = glm::vec3(19,-3.5, 0);
//    glm::vec3 tutorial_counter_loc = glm::vec3(24, -3.5, 0);
//
//    Util::DrawText(program, fontTextureID, "You have limited shots", 0.25f, -.1, reminder);
//    Util::DrawText(program, fontTextureID, "so choose your fights!", 0.25f, -.1, reminder_1);
//    Util::DrawText(program, fontTextureID, "Use SPACE to shoot", 0.25f, -.1, tutorial_shoot_loc);
//    Util::DrawText(program, fontTextureID, "You can shoot enemy projectiles", 0.25f, -.1, tutorial_counter_loc);
    
    for (int i = 0; i < LEVEL4_ENEMY_COUNT; ++i) {
        Entity* enemy = &state.enemies[i];
        enemy->Render(program);
        enemy->projectile->Render(program);
    }
    glm::vec3 text_loc = state.player->position;
    text_loc.x -= .75;
    text_loc.y += .75;
    glm::vec3 announce_loc = text_loc;
    announce_loc.y += 1.5;
    switch(state.livesLeft) {
        case 2:
            Util::DrawText(program, fontTextureID, "Lives Left: 2", 0.25f, -.1, text_loc);
            break;
        case 1:
            Util::DrawText(program, fontTextureID, "Lives Left: 1", 0.25f, -.1, text_loc);
            break;
        case 0:
            Util::DrawText(program, fontTextureID, "Lives Left: 0", 0.25f, -.1, text_loc);
            break;
    }
    text_loc.y += .35;
    switch(state.shotsLeft) {
        case 2:
            Util::DrawText(program, fontTextureID, "Shots Left: 2", 0.25f, -.1, text_loc);
            break;
        case 1:
            Util::DrawText(program, fontTextureID, "Shots Left: 1", 0.25f, -.1, text_loc);
            break;
        case 0:
            Util::DrawText(program, fontTextureID, "Shots Left: 0", 0.25f, -.1, text_loc);
            break;
    }
    if (state.control == GAME_LOST)
        Util::DrawText(program, fontTextureID, "You Lose!", 0.25f, -.1, announce_loc);
    else if (state.control == GAME_WON)
        Util::DrawText(program, fontTextureID, "You Win!", 0.25f, -.1, announce_loc);
}
