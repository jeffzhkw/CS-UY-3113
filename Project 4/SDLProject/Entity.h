//
//  Entity.hpp
//  Project4
//
//  Created by Kewei Zhang on 7/11/20.
//  Copyright © 2020 ctg. All rights reserved.
//

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

enum EntityType{PLAYER, BLOCK,ENEMY};


enum AIType {TRACK, WALK, DASH };
enum AIState {IDLE, SENSING, ENGAGE, DEAD};

class Entity {
public:
    EntityType entityType;
    AIType aiType;
    AIState aiState;
    
    glm::vec3 position;
    glm::vec3 movement;
    glm::vec3 acceleration;
    glm::vec3 velocity;
    float speed;
    
    float width = 1;
    float height = 1;
    
    bool jump = false;
    float jumpPower = 0;
    
    GLuint textureID;
    glm::mat4 modelMatrix;
    
    int *animRight = NULL;
    int *animLeft = NULL;
    int *animUp = NULL;
    int *animDown = NULL;

    int *animIndices = NULL;
    int animFrames = 0;
    int animIndex = 0;
    float animTime = 0;
    int animCols = 0;
    int animRows = 0;
    
    bool isActive = true;
    bool collidedTop = false;
    bool collidedBottom = false;
    bool collidedLeft = false;
    bool collidedRight = false;
    EntityType lastCollide;
    
    int fontIndex = NULL;
    
    Entity();
    
    bool CheckCollision(Entity *other);
    void CheckCollisionsY(Entity *objects, int objectCount);
    void CheckCollisionsX(Entity *objects, int objectCount);
    
    bool pointInBox(glm::vec3 point, Entity *other);
    void CheckForPit(Entity *objects, int objectCount);
    
    void Update(float deltaTime, Entity *player, Entity *platform, int platformCount);
    void DrawSpriteFromTextureAtlas(ShaderProgram *program, GLuint textureID, int index);
    void Render(ShaderProgram *program);

    
    void AI(Entity *player);
    void AITrack(Entity *player);
    void AIWalk(Entity *player);
    void AIDash(Entity *player);
};


