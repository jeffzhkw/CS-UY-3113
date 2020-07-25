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
//Entity vs Entity
bool Entity::CheckCollision(Entity *other){
    if (this == other) return false;
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
//Entity vs Map
void Entity::CheckCollisionY(Map *map){
    // Probes for tiles
    glm::vec3 top = glm::vec3(position.x, position.y + (height / 2), position.z);
    glm::vec3 top_left = glm::vec3(position.x - (width / 2), position.y + (height / 2), position.z);
    glm::vec3 top_right = glm::vec3(position.x + (width / 2), position.y + (height / 2), position.z);
    
    glm::vec3 bottom = glm::vec3(position.x, position.y - (height / 2), position.z);
    glm::vec3 bottom_left = glm::vec3(position.x - (width / 2), position.y - (height / 2), position.z);
    glm::vec3 bottom_right = glm::vec3(position.x + (width / 2), position.y - (height / 2), position.z);
    
    float penetration_x = 0; float penetration_y = 0;
    
    if (map->IsSolid(top, &penetration_x, &penetration_y) && velocity.y > 0) {
        position.y -= penetration_y;
        velocity.y = 0;
        collidedTop = true;
    }
    else if (map->IsSolid(top_left, &penetration_x, &penetration_y) && velocity.y > 0) {
        position.y -= penetration_y;
        velocity.y = 0;
        collidedTop = true;
    }
    else if (map->IsSolid(top_right, &penetration_x, &penetration_y) && velocity.y > 0) {
        position.y -= penetration_y;
        velocity.y = 0;
        collidedTop = true;
    }

    if (map->IsSolid(bottom, &penetration_x, &penetration_y) && velocity.y < 0) {
        position.y += penetration_y;
        velocity.y = 0;
        collidedBottom = true;
    }
    else if (map->IsSolid(bottom_left, &penetration_x, &penetration_y) && velocity.y < 0) {
        position.y += penetration_y;
        velocity.y = 0;
        collidedBottom = true;
    }
    else if (map->IsSolid(bottom_right, &penetration_x, &penetration_y) && velocity.y < 0) {
        position.y += penetration_y;
        velocity.y = 0;
        collidedBottom = true;
    }
}

void Entity::CheckCollisionX(Map *map){
    // Probes for tiles
    glm::vec3 left = glm::vec3(position.x - (width / 2), position.y, position.z);
    glm::vec3 right = glm::vec3(position.x + (width / 2), position.y, position.z);
    
    float penetration_x = 0; float penetration_y = 0;
    
    if (map->IsSolid(left, &penetration_x, &penetration_y) && velocity.x < 0) {
        position.x += penetration_x;
        velocity.x = 0;
        collidedLeft = true;
    }
    if (map->IsSolid(right, &penetration_x, &penetration_y) && velocity.x > 0) {
        position.x -= penetration_x;
        velocity.x = 0;
        collidedRight = true;
    }
}





void Entity::AIDash(Entity *player){
    switch(aiState){
        case IDLE:
            std::cout << "IDLE" <<std::endl;
            if (glm::distance(position, player->position) < 7){
                aiState = SENSING;
                velocity.x = -1;
            }
            break;
        case SENSING:
            std::cout << "SENSING" <<std::endl;
            if (glm::distance(position, player->position) < 5){
                aiState = ENGAGE;
            }
            break;
        case ENGAGE:
            std::cout << "ENGAGE" <<std::endl;
            if (player->position.x < position.x){
                acceleration.x = -2;
            }
            else if (player->position.x > position.x){
                acceleration.x = 2;
            }
            std::cout<< collidedBottom <<std::endl;
            if ((player->position.y > position.y) && collidedBottom){
                velocity.y += jumpPower;
            }
            break;
        case DEAD:
            break;
    }
}

void Entity::AI(Entity *player){
    switch(aiType){
        case DASH:
            AIDash(player);
            break;
    }
}



void Entity::Update(float deltaTime, Entity *player, Entity *objects, int objectCount, Map *map){
    if (isActive == false) return;
    
    collidedTop = false;
    collidedBottom = false;
    collidedLeft = false;
    collidedRight = false;
    lastCollide = NONE;
    if (entityType == ENEMY){
        velocity += acceleration * deltaTime;
        
        position.y += velocity.y * deltaTime;
        CheckCollisionY(map);
        
        position.x += velocity.x * deltaTime;
        CheckCollisionX(map);
        AI(player);
//        if (lastCollide == PLAYER && position.x < player->position.x){
//            isActive = false;
//        }
        modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::translate(modelMatrix, position);
        
        
    }
    
    else if (entityType == PLAYER){
        if (animIndices != NULL) {
            if (glm::length(movement) != 0) {
                animTime += deltaTime;
                if (animTime >= 0.25f){
                    animTime = 0.0f;
                    animIndex++;
                    if (animIndex >= animFrames){
                        animIndex = 0;
                    }
                }
            }
            else {
                animIndex = 0;
            }
        }
        if (jump){
            jump = false;
            velocity.y += jumpPower;
        }
        velocity.x = movement.x * speed;
        velocity += acceleration * deltaTime;
        
        position.y += velocity.y * deltaTime;
        CheckCollisionY(map);
        CheckCollisionsY(objects, objectCount);//check velocity, set to zero
        
        position.x += velocity.x * deltaTime;
        CheckCollisionX(map);
        CheckCollisionsX(objects, objectCount);
        
        if (lastCollide == ENEMY){
            isActive = false;
        }
        
        modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::translate(modelMatrix, position);
        
        
    }
    else if (entityType == NONE){
        modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::translate(modelMatrix, position);
    }
    
}
//for player animation
void Entity::DrawSpriteFromTextureAtlas(ShaderProgram *program, GLuint textureID, int index)
{
    float u = (float)(index % animCols) / (float)animCols;
    float v = (float)(index / animCols) / (float)animRows;
    
    float width = 1.0f / (float)animCols;
    float height = 1.0f / (float)animRows;
    
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
    
    if (animIndices != NULL) {
        DrawSpriteFromTextureAtlas(program, textureID, animIndices[animIndex]);
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







