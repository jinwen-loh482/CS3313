#include "Level2.h"
#define Level2_WIDTH 27
#define Level2_HEIGHT 9

#define Level2_ENEMY_COUNT 1

unsigned int Level2_data[] =
{
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0,     0, 0, 0, 0, 0, 0, 0, 0, 0, 0,     0, 0, 0, 0, 0, 0, 3,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0,     0, 0, 0, 0, 0, 0, 0, 0, 0, 0,     0, 0, 0, 0, 0, 0, 3,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0,     0, 0, 0, 0, 0, 0, 0, 0, 0, 0,     0, 0, 0, 0, 0, 0, 3,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0,     0, 0, 0, 0, 0, 0, 0, 0, 0, 0,     0, 0, 0, 0, 0, 0, 3,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0,     0, 0, 0, 0, 0, 0, 0, 0, 0, 0,     0, 0, 0, 0, 0, 0, 3,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0,     0, 0, 0, 0, 0, 0, 0, 0, 0, 0,     0, 0, 0, 0, 0, 0, 3,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0,     0, 0, 0, 0, 0, 0, 0, 0, 0, 0,     0, 0, 1, 3, 3, 3, 3,
    3, 1, 1, 1, 1, 1, 1, 1, 0, 0,     1, 1, 1, 1, 1, 1, 0, 0, 1, 1,     1, 1, 2, 2, 2, 2, 3,
    3, 2, 2, 2, 2, 2, 2, 2, 0, 0,     2, 1, 1, 2, 2, 2, 0, 0, 2, 2,     2, 2, 2, 2, 2, 2, 3
};

void Level2::Initialize(int lives) {
    state.livesLeft = lives;
    state.sceneNumber = 2;
    state.nextScene = -1;
    state.control = IN_PROGRESS;
    GLuint mapTextureID = Util::LoadTexture("Assets/Textures/tileset.png");
    fontTextureID = Util::LoadTexture("Assets/Textures/font1.png");
    state.map = new Map(Level2_WIDTH, Level2_HEIGHT, Level2_data, mapTextureID, 1.0f, 4, 1);
    // Move over all of the player and enemy code from initialization.
    // Initialize Player
    state.player = new Entity();
    state.player->entityType = PLAYER;
    state.player->position = glm::vec3(3, -5, 0);
    state.player->movement = glm::vec3(0);
    state.player->acceleration = glm::vec3(0, -9.81f, 0);
    state.player->speed = 3.0f;
    state.player->textureID = Util::LoadTexture("Assets/Textures/george_0.png");
    
    state.player->animRight = new int[4] {3, 7, 11, 15};
    state.player->animLeft = new int[4] {1, 5, 9, 13};
    state.player->animUp = new int[4] {2, 6, 10, 14};
    state.player->animDown = new int[4] {0, 4, 8, 12};

    state.player->animIndices = state.player->animRight;
    state.player->animFrames = 4;
    state.player->animIndex = 0;
    state.player->animTime = 0;
    state.player->animCols = 4;
    state.player->animRows = 4;
    
    state.player->height = 0.8f;
    state.player->width = 0.8f;
    
    state.player->jumpPower = 6.0f;
    
    
    state.enemies = new Entity[Level2_ENEMY_COUNT];
    GLuint enemyTextureID = Util::LoadTexture("Assets/Textures/ctg.png");
    
    state.enemies[0].entityType = ENEMY;
    state.enemies[0].textureID = enemyTextureID;
    state.enemies[0].position = glm::vec3(12, -5, 0);
    state.enemies[0].speed = 1;
    state.enemies[0].movement = glm::vec3(0);
    state.enemies[0].acceleration = glm::vec3(0, -9.81f, 0);
    state.enemies[0].aiType = WALKER;
    state.enemies[0].aiState = ACTIVE;
    state.enemies[0].isActive = true;
}
void Level2::Update(float deltaTime) {
    if (state.control == IN_PROGRESS) {
        state.player->Update(deltaTime, state.player, state.enemies, Level2_ENEMY_COUNT, state.map, state.sceneNumber);
        for (int i = 0; i < Level2_ENEMY_COUNT; ++i) {
            Entity* enemy = &state.enemies[i];
            enemy->Update(deltaTime, state.player, state.enemies, Level2_ENEMY_COUNT, state.map, state.sceneNumber);
        }
        
        if (state.player->position.y <= -9)
            state.player->isActive = false;
        
        if (state.player->position.x >= 23) {
            state.nextScene = 3;
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
void Level2::Render(ShaderProgram *program) {
    state.map->Render(program);
    state.player->Render(program);
    for (int i = 0; i < Level2_ENEMY_COUNT; ++i) {
        Entity* enemy = &state.enemies[i];
        enemy->Render(program);
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
