//
//  Level1.cpp
//  Project5
//
//  Created by Kewei Zhang on 7/21/20.
//  Copyright © 2020 ctg. All rights reserved.
//
#include "Level1.h"
#define LEVEL1_WIDTH 22
#define LEVEL1_HEIGHT 14

#define LEVEL1_ENEMY_COUNT 0
unsigned int level1_data[] = {
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 3,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 1, 3,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 2, 3,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 1, 1, 0, 0, 0, 0, 1, 2, 3,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 1, 2, 2, 3,
    3, 1, 1, 1, 1, 1, 2, 2, 2, 0, 0, 0, 3, 0, 0, 0, 0, 1, 2, 3, 3, 3,
    3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 3,
    3, 3, 3, 3, 3, 3, 3, 3, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
    3, 3, 3, 3, 3, 3, 3, 3, 2, 2, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 3,
    3, 2, 2, 2, 2, 3, 3, 3, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
    3, 2, 2, 2, 2, 2, 3, 3, 2, 2, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 3
};//hardcode corresponding value;
glm::mat4 viewMatrix1;
void Level1::Initialize(){
    //init player/enemy/map
    state.nextScene = -1;
    GLuint mapTextureID = Util::LoadTexture("tileset.png");
    state.map = new Map(LEVEL1_WIDTH, LEVEL1_HEIGHT, level1_data, mapTextureID, 1.0f, 4, 1);
    state.player = new Entity();
    state.player->entityType = PLAYER;
    state.player->position = glm::vec3(5,-3.75,0);
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
    
}

void Level1::Update(float deltaTime){
    state.player->Update(deltaTime, state.player, state.enemies, LEVEL1_ENEMY_COUNT, state.map);
    
    //update viewMatrix to follow the player
    
    

    
    std::cout << state.player->position.x << std::endl;
    std::cout << state.player->position.y << std::endl;
    
    //switch scene condition
    
    if (state.player->position.x >=19.5 && state.player->position.y < -12){
        state.nextScene = 2;//LEVEL2
    }
    
}

void Level1::Render(ShaderProgram *program){
    //program->SetViewMatrix(viewMatrix1);
    state.map->Render(program);
    state.player->Render(program);
}
