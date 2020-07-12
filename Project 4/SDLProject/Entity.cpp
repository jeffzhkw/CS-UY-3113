//
//  Entity.cpp
//  Project4
//
//  Created by Kewei Zhang on 7/11/20.
//  Copyright © 2020 ctg. All rights reserved.
//

#include "Entity.h"

Entity::Entity()
{
    position = glm::vec3(0);
    movement = glm::vec3(0);
    acceleration = glm::vec3(0);
    velocity = glm::vec3(0);
    speed = 0;
    
    modelMatrix = glm::mat4(1.0f);
}

//check Collisions
bool Entity::CheckCollision(Entity *other){
    if (isActive == false || other->isActive == false) return false;
    float xdist = fabs(position.x - other->position.x) - ((width + other->width)/2.0f);
    float ydist = fabs(position.y - other->position.y) - ((height + other->height)/2.0f);
    
    if (xdist < 0  && ydist < 0){
        lastCollide = other->entityType;
        return true;
    }
    return false;
}

void Entity::CheckCollisionsY(Entity *objects, int objectCount){
    for (int i =0; i < objectCount; i++){
        Entity *object = &objects[i];
        
        if (CheckCollision(object)){
            float ydist = fabs(position.y - object->position.y);
            float penetrationY = fabs(ydist - ((height + object->height)/2.0f));
            if (velocity.y > 0){
                position.y -= penetrationY;
                velocity.y = 0;
                collidedTop = true;
            }
            else if (velocity.y <0){
                position.y += penetrationY;
                velocity.y = 0;
                collidedBottom = true;
            }
        }
    }
}

void Entity::CheckCollisionsX(Entity *objects, int objectCount){
    for (int i =0; i < objectCount; i++){
        Entity *object = &objects[i];
        
        if (CheckCollision(object)){
            float xdist = fabs(position.x - object->position.x);
            float penetrationX = fabs(xdist - ((width + object->width)/2.0f));
            if (velocity.x > 0){
                position.x -= penetrationX;
                velocity.x = 0;
                collidedRight = true;
            }
            else if (velocity.x <0){
                position.x += penetrationX;
                velocity.x = 0;
                collidedLeft = true;
            }
        }
    }
}
//bool Entity::pointInBox(glm::vec3 point, Entity *other){
//
//    glm::vec3 topLeft = glm::vec3(other->position.x-other->width/2, other->position.y + other->height/2, 0);
//    glm::vec3 bottomRight = glm::vec3(other->position.x + other->width/2, other->position.y - other->height/2, 0);
//
//    if (topLeft.x < point.x &&point.x < bottomRight.x){
//        if (bottomRight.y < point.y && point.y < topLeft.y){
//            return true;
//        }
//    }
//    return false;
//}
//
//void Entity::CheckForPit(Entity *objects, int objectCount){
//    //two sensor point
//    glm::vec3 rightSensor = glm::vec3(position.x + 0.6f, position.y -0.6f, 0);
//    glm::vec3 leftSensor = glm::vec3(position.x -0.6f, position.y -0.6f,0);
//
//    for (int i = 0; i < objectCount; i++){
//        Entity *object = &objects[i]; //the block
//
//        //if sensor point not in block
//        if(pointInBox(rightSensor, object) == false){
//            collidedRight = true;
//        }
//        else if (pointInBox(leftSensor, object) == false){
//            collidedLeft = true;
//        }
//    }
//}

//AI Methods
void Entity::AIWalk(Entity *player){
    
    switch(aiState){
        case IDLE:
            if (glm::distance(position, player->position) < 8.0f){
                aiState = SENSING;
                movement = glm::vec3(-1,0,0);
            }
            break;
        case SENSING:
            if (collidedRight || collidedLeft){
                movement.x = movement.x * -1;
            }
            break;
        case ENGAGE:
            break;
        case DEAD:
            break;
    }
}

void Entity::AITrack(Entity *player){
    switch(aiState){
        case IDLE:
            if (glm::distance(position, player->position) < 3.0f){
                movement = glm::vec3(-1,0,0);
                aiState = SENSING;
            }
            break;
        case SENSING:
            speed = 1.5f;
            if (player->position.x > position.x){
                movement = glm::vec3(1,0,0);
            }
            else if (player->position.x < position.x){
                movement = glm::vec3(-1,0,0);
            }
            break;
        case ENGAGE:
            //jumping around
            break;
        case DEAD:
            break;
    }
}

void Entity::AIDash(Entity *player){
    switch(aiState){
        case IDLE:
            if (glm::distance(position, player->position) < 20.0f){
                aiState = SENSING;
                 movement = glm::vec3(1,0,0);
            }
            break;
        case SENSING:
            if (glm::distance(position.y, player->position.y) <0.1f){//same horizontal
                aiState = ENGAGE;
            }
            std::cout << "right: " << collidedRight << std::endl;
            if (collidedRight || collidedLeft){
                movement.x = movement.x * -1;
            }
            break;
        case ENGAGE:
            speed = 5.0f;
            if (collidedRight || collidedLeft){
                movement.x = movement.x * -1;
            }
            if (glm::distance(position.y, player->position.y) <1.5f){//same horizontal
                aiState = SENSING;
            }
            
            break;
        case DEAD:
            break;
    }
}

void Entity::AI(Entity *player){
    switch(aiType){
        case WALK:
            std::cout << "WALK" << std::endl;
            AIWalk(player);
            break;
        case TRACK:
            std::cout << "TRACK" << std::endl;
            AITrack(player);
            break;
        case DASH:
            std::cout << "DASH" << std::endl;
            AIDash(player);
            break;
    }
    if (CheckCollision(player)){
        if (player->position.y > position.y){
            isActive = false;
        }
        else{
            player->isActive = false;
        }
    }
}

//Update and Render, can be called by PLAYER, ENEMY, BLOCK
void Entity::Update(float deltaTime, Entity *player, Entity *platforms, int platformCount){
    if (isActive == false) return;
    
    collidedTop = false;
    collidedBottom = false;
    collidedLeft = false;
    collidedRight = false;
    
    
    
    //This part is for checking PLAYER/ENEMY against BLOCKS
    velocity.x = movement.x * speed;
    velocity += acceleration * deltaTime;
    
    position.y += velocity.y * deltaTime;
    CheckCollisionsY(platforms, platformCount);//check velocity, set to zero
    
    position.x += velocity.x * deltaTime;
    CheckCollisionsX(platforms, platformCount);
    
    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, position);
    
    if (entityType == ENEMY){
        //CheckForPit(platforms, platformCount);
        AI(player);
        //ENEMY check whether it hits player.
        //1. this.lastCollide = PLAYER
        //2. top-> DEAD, left right->GAME END
        //3. 3 ENEMIES DEAD->WIN
    }
    if (entityType == PLAYER){
        if (jump){
            jump = false;
            velocity.y += jumpPower;
        }
    }
    
}
void Entity::DrawSpriteFromTextureAtlas(ShaderProgram *program, GLuint textureID, int index)
{
    float u = (float)(index % 16) / (float)16;
    float v = (float)(index / 16) / (float)16;
    
    float width = 1.0f / (float)16;
    float height = 1.0f / (float)16;
    
    float texCoords[] = { u, v + height, u + width, v + height, u + width, v,
        u, v + height, u + width, v, u, v};
    
    float vertices[]  = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
    
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program->positionAttribute);
    
    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program->texCoordAttribute);
    
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
}

void Entity::Render(ShaderProgram *program) {
    if (isActive == false) return;
    
    program->SetModelMatrix(modelMatrix);
    
    if (fontIndex != NULL) {//is a letter
        DrawSpriteFromTextureAtlas(program, textureID, fontIndex);//need init. in main
        return;
    }
    
    
    float vertices[]  = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
    float texCoords[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };
    
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program->positionAttribute);
    
    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program->texCoordAttribute);
    
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
}
