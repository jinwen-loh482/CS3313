#include "Level1.h"
#define LEVEL1_WIDTH 19
#define LEVEL1_HEIGHT 8

#define LEVEL1_ENEMY_COUNT 1

#define WALL 10

unsigned int level1_data[] =
{
    10, 10, 10, 10, 10, 10, 10, 10, 10, 10,     10, 10, 10, 10, 10, 10, 10, 10, 10,
    10,  0,  0,  0,  0,  0,  0,  0,  0,  0,     0,  0,  0,  0,  0,  0,  0,  0,  0,
    10,  0,  0,  0,  0,  0,  0,  0,  0,  0,     0,  0,  0,  0,  0,  0,  0,  0,  0,
    10,  0,  0,  0,  0,  0,  0,  0,  0,  0,     0,  0,  0,  0,  0,  0,  0,  0,  0,
    10,  0,  0,  0,  0,  0,  0,  0,  0,  0,     0,  0,  0,  0,  0,  0,  0,  0,  0,
    10,  0,  0,  0,  0,  0,  0,  0,  0,  0,     0,  0,  0,  0,  0,  0,  0,  0,  0,
    10,  0,  0,  0,  0,  0,  0,  0,  0,  0,     0,  0,  0,  0,  0,  0,  0,  0,  0,
    10, 10, 10, 10, 10, 10, 10, 10, 10, 10,     10, 10, 10, 10, 10, 10, 10, 10, 10

};

void Level1::Initialize(int lives) {
    state.livesLeft = lives;
    state.sceneNumber = 1;
    state.nextScene = -1;
    state.control = IN_PROGRESS;
    GLuint mapTextureID = Util::LoadTexture("Assets/Textures/monochrome_tilemap.png");
    fontTextureID = Util::LoadTexture("Assets/Textures/font1.png");
    state.map = new Map(LEVEL1_WIDTH, LEVEL1_HEIGHT, level1_data, mapTextureID, 1.0f, 20, 20);
    // Move over all of the player and enemy code from initialization.
    // Initialize Player
    state.player = new Entity();
    state.player->entityType = PLAYER;
    state.player->position = glm::vec3(5, -5, 0);
    state.player->movement = glm::vec3(0, 0, 0);
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
    
    
    state.enemies = new Entity[LEVEL1_ENEMY_COUNT];
    GLuint enemyTextureID = Util::LoadTexture("Assets/Textures/enemy.png");
//
    state.enemies[0].entityType = ENEMY;
    state.enemies[0].textureID = enemyTextureID;
    state.enemies[0].position = glm::vec3(10, -5, 0);
    state.enemies[0].speed = 1;
//    state.enemies[0].aiType = WALKER;
//    state.enemies[0].aiState = ACTIVE;
    state.enemies[0].isActive = true;
    state.enemies[0].projectile = new Entity();
    state.enemies[0].projectile->entityType = PROJECTILE;
    state.enemies[0].projectile->isActive = true;
    state.enemies[0].projectile->position = state.enemies[0].position;
    state.enemies[0].projectile->movement = glm::vec3(-1, 0, 0);
    state.enemies[0].projectile->textureID = Util::LoadTexture("Assets/Textures/projectile.png");
    state.enemies[0].projectile->width = 0.8f;
    state.enemies[0].projectile->height = 0.8f;
    
}
void Level1::Update(float deltaTime) {
    if (state.control == IN_PROGRESS) {
        state.player->Update(deltaTime, state.player, state.enemies, LEVEL1_ENEMY_COUNT, state.map, state.sceneNumber);
        for (int i = 0; i < LEVEL1_ENEMY_COUNT; ++i) {
            Entity* enemy = &state.enemies[i];
            enemy->Update(deltaTime, enemy, state.player, 1, state.map, state.sceneNumber);
        }
        
        // Update player projectile
        state.player->projectile->Update(deltaTime, state.player->projectile, state.enemies, LEVEL1_ENEMY_COUNT, state.map, state.sceneNumber);
        for (int i = 0; i < LEVEL1_ENEMY_COUNT; ++i) {
            Entity* enemy = &state.enemies[i];
            enemy->projectile->Update(deltaTime, enemy->projectile, state.player, 1, state.map, state.sceneNumber);
        }
        if (state.player->position.y > -4) {
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
void Level1::Render(ShaderProgram *program) {
    state.map->Render(program);
    state.player->Render(program);
    state.player->projectile->Render(program);
    
    for (int i = 0; i < LEVEL1_ENEMY_COUNT; ++i) {
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
    if (state.control == GAME_LOST)
        Util::DrawText(program, fontTextureID, "You Lose!", 0.25f, -.1, announce_loc);
    else if (state.control == GAME_WON)
        Util::DrawText(program, fontTextureID, "You Win!", 0.25f, -.1, announce_loc);
}
