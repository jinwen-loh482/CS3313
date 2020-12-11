#include "Level0.h"

void Level0::Initialize(int lives) {
    state.player = new Entity();
    state.player->isActive = false;
    state.nextScene = -1;
    state.sceneNumber = 0;
    state.livesLeft = 2;
    fontTextureID = Util::LoadTexture("Assets/Textures/font1.png");
}

void Level0::Update(float deltaTime) {
    if (state.control == IN_PROGRESS) state.nextScene = 1;
}

void Level0::Render(ShaderProgram *program) {
    Util::DrawText(program, fontTextureID, "Project 6", 0.5f, -.1, glm::vec3(-1.75, 1, 0));
    Util::DrawText(program, fontTextureID, "Out-of-Place Michael Jackson ", .35f, -.1, glm::vec3(-3.5f, 0, 0));
    Util::DrawText(program, fontTextureID, "Sidescroll Shooter", .35f, -.1, glm::vec3(-2.5f, -0.5, 0));
    Util::DrawText(program, fontTextureID, "Press ENTER", 0.5f, -.1, glm::vec3(-2.1, -1.5, 0));
}

//#define LEVEL1_WIDTH 14
//#define LEVEL1_HEIGHT 8
//
//#define LEVEL1_ENEMY_COUNT 1
//
//unsigned int level0_data[] =
//{
//    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//    3, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1,
//    3, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2,
//    3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2
//};
//
//void Level0::Initialize() {
//    state.sceneNumber = 1;
//    state.nextScene = -1;
//    GLuint mapTextureID = Util::LoadTexture("Assets/Textures/tileset.png");
//    state.map = new Map(LEVEL1_WIDTH, LEVEL1_HEIGHT, level0_data, mapTextureID, 1.0f, 4, 1);
//    // Move over all of the player and enemy code from initialization.
//    // Initialize Player
//    state.player = new Entity();
//    state.player->entityType = PLAYER;
//    state.player->position = glm::vec3(5, 0, 0);
//    state.player->movement = glm::vec3(0);
//    state.player->acceleration = glm::vec3(0, -9.81f, 0);
//    state.player->speed = 2.0f;
//    state.player->textureID = Util::LoadTexture("Assets/Textures/george_0.png");
//
//    state.player->animRight = new int[4] {3, 7, 11, 15};
//    state.player->animLeft = new int[4] {1, 5, 9, 13};
//    state.player->animUp = new int[4] {2, 6, 10, 14};
//    state.player->animDown = new int[4] {0, 4, 8, 12};
//
//    state.player->animIndices = state.player->animRight;
//    state.player->animFrames = 4;
//    state.player->animIndex = 0;
//    state.player->animTime = 0;
//    state.player->animCols = 4;
//    state.player->animRows = 4;
//
//    state.player->height = 0.8f;
//    state.player->width = 0.8f;
//
//    state.player->jumpPower = 6.0f;
//
//
//    state.enemies = new Entity[LEVEL1_ENEMY_COUNT];
//    GLuint enemyTextureID = Util::LoadTexture("Assets/Textures/ctg.png");
//
//    state.enemies[0].entityType = ENEMY;
//    state.enemies[0].textureID = enemyTextureID;
//    state.enemies[0].position = glm::vec3(4, -2.2, 0);
//    state.enemies[0].speed = 1;
//    state.enemies[0].aiType = WAITANDGO;
//    state.enemies[0].aiState = IDLE;
//    state.enemies[0].isActive = false;
//}
//void Level0::Update(float deltaTime) {
//    state.player->Update(deltaTime, state.player, state.enemies, LEVEL1_ENEMY_COUNT, state.map);
//
//    if (state.player->position.x >= 12)
//        state.nextScene = 1;
//}
//
//void Level0::Render(ShaderProgram *program) {
//    state.map->Render(program);
//    state.player->Render(program);
//}
