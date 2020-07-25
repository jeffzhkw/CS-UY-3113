//
//  Level3.cpp
//  Project5
//
//  Created by Kewei Zhang on 7/21/20.
//  Copyright © 2020 ctg. All rights reserved.
//
#include "Level3.h"
#define LEVEL3_WIDTH 27
#define LEVEL3_HEIGHT 9

#define LEVEL3_ENEMY_COUNT 1

unsigned int level3_data[] = {
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
    3, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3,
    3, 0, 0, 0, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3,
    3, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3
};//hardcode corresponding value;
Entity *door3;
void Level3::Initialize(){
    //init player/enemy
    
    state.nextScene = -1;
    GLuint mapTextureID = Util::LoadTexture("tileset.png");
    state.map = new Map(LEVEL3_WIDTH, LEVEL3_HEIGHT, level3_data, mapTextureID, 1.0f, 4, 1);
    
    state.player = new Entity();
    state.player->entityType = PLAYER;
    state.player->position = glm::vec3(1,-7,0);
    state.player->movement = glm::vec3(0);
    state.player->acceleration = glm::vec3(0, -9.82f, 0);
    state.player->speed = 3.0f;
    state.player->jumpPower = 10.0f;
    state.player->textureID = Util::LoadTexture("george_0.png");
    
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
    
    state.enemies = new Entity();
    state.enemies->entityType = ENEMY;
    state.enemies->textureID = Util::LoadTexture("ctg.png");
    state.enemies->acceleration = glm::vec3(0, -9.8f, 0);
    state.enemies->speed = 1.0f;
    state.enemies->movement = glm::vec3(0);
    state.enemies->position = glm::vec3(12,-4,0);
    state.enemies->jumpPower = 5.0f;
    state.enemies->aiType = DASH;
    state.enemies->aiState = IDLE;
    
    door3 = new Entity();
    door3->entityType = NONE;
    door3->position = glm::vec3 (24.5, -5, 0);
    door3->textureID = Util::LoadTexture("door.png");
    
}

void Level3::Update(float deltaTime){
    state.player->Update(deltaTime, state.player, state.enemies, LEVEL3_ENEMY_COUNT, state.map);
    
    door3->Update(deltaTime, state.player, state.enemies, LEVEL3_ENEMY_COUNT, state.map);
    
    state.enemies->Update(deltaTime, state.player, state.enemies, LEVEL3_ENEMY_COUNT, state.map);
    std::cout<< state.enemies->position.x <<std::endl;
    std::cout<< state.enemies->position.y <<std::endl;
    
}

void Level3::Render(ShaderProgram *program){
    state.map->Render(program);
    state.enemies->Render(program);
    door3->Render(program);
    Util::DrawText(program, Util::LoadTexture("font.png"), "RUN!", 0.5, -0.25, glm::vec3(3,-3.75,0));
    Util::DrawText(program, Util::LoadTexture("font.png"), "Don't even think about finghting!!!", 0.5, -0.25, glm::vec3(1,-1.75,0));
    state.player->Render(program);
}
