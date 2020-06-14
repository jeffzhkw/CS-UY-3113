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

ShaderProgram program;

SDL_Window* displayWindow;
bool gameIsRunning = true;

glm::mat4 viewMat, projectionMat;


glm::mat4 p1, p2, ball;
glm::vec3 p1_pos, p2_pos, ball_pos;//regard it as a position.
glm::vec3 p1_move, p2_move, ball_move;//as having a tendency or not


void Initialize(){
    SDL_Init(SDL_INIT_VIDEO);
        displayWindow = SDL_CreateWindow("Pong! Made by kw", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
        SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
        SDL_GL_MakeCurrent(displayWindow, context);
        
    #ifdef _WINDOWS
        glewInit();
    #endif
    
    program.Load("shaders/vertex.glsl", "shaders/fragment.glsl");

    program.SetViewMatrix(glm::mat4(1.0f));
    program.SetProjectionMatrix(glm::ortho(-5.0f,5.0f,-3.75f,3.75f,-1.0f, 1.0f));
    
    p1 = glm::mat4(1.0f);
    p2 = glm::mat4(1.0f);
    ball = glm::mat4(1.0f);
    p1_pos.x = -4.5f;
    p2_pos.x = 4.5f;
    
    
   glClearColor(0.945f, 0.863f, 0.612f, 1.0f);
        
    
}

void ProcessInput(){
    p1_move = glm::vec3(0);
    p2_move = glm::vec3(0);
    
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch(event.type){
            case SDL_QUIT:
            case SDL_WINDOWEVENT_CLOSE:
                gameIsRunning = false;
            case SDL_KEYDOWN:
                switch(event.key.keysym.sym){
                    case SDLK_SPACE:
                        ball_move.x = -0.5f;
                        ball_move.y = -1.0f;
                        break;
                }
        }
    }
    const Uint8* keys = SDL_GetKeyboardState(NULL);
    
    if(keys[SDL_SCANCODE_W]){
        p1_move.y = 1.0f;
    }
    else if (keys[SDL_SCANCODE_S]){
        p1_move.y = -1.0f;
    }
    if(keys[SDL_SCANCODE_UP]){
        p2_move.y = 1.0f;
    }
    else if (keys[SDL_SCANCODE_DOWN]){
        p2_move.y = -1.0f;
    }
    

}
float lastTick = 0;
float player_speed = 5.0f;
float ball_speed = 4.0f;
void Update(){
    p1 = glm::mat4(1.0f);
    p2 = glm::mat4(1.0f);
    ball = glm :: mat4(1.0f);//reset complete
    
    float ticks = (float)SDL_GetTicks() /1000.0f;
    float deltaTime = ticks - lastTick;
    lastTick = ticks;

    if (p1_pos.y>3.15f || p1_pos.y < -3.15f){//constrains for paddles
        p1_move = glm::vec3(0);
        if(p1_pos.y > 0) p1_pos.y = 3.15f;
        else p1_pos.y = -3.15f;
    }
    if (p2_pos.y>3.15f || p2_pos.y < -3.15f){
        p2_move = glm::vec3(0);
        if(p2_pos.y > 0) p2_pos.y = 3.15f;
        else p2_pos.y = -3.15f;
    }
    if (ball_pos.y > 3.65f || ball_pos.y < -3.65f){//constrains for ball
        ball_move.y *= -1.0f;
        //prevent sticky bug
        if (ball_pos.y > 3.65f) ball_pos.y = 3.65f;
        else ball_pos.y = -3.65f;
    }
    //x threhold: 0.25f; y threhold 0.7f;
    //can refine collision
    if (fabs(p1_pos.x - ball_pos.x) < 0.25f && fabs(p1_pos.y - ball_pos.y)<0.7f){//p1 horizontal&vertical collide check
        if (ball_pos.x > -4.5f) {
            ball_move.x *= -1.0f;
            ball_pos.x = -4.25f;
        }
        else ball_speed = 1.0f;
    }
    if (fabs(p2_pos.x - ball_pos.x) < 0.25f && fabs(p2_pos.y - ball_pos.y)<0.7f){//p2 horizontal&vertical collide check
        if (ball_pos.x < 4.5f) {
            ball_move.x *= -1.0f;
            ball_pos.x = 4.25f;
        }
        
        else ball_speed = 1.0f;
    }
    
    //losing animation
    if (ball_pos.x < -4.65f){std::cout<<"p1 defeated" << std::endl;ball_speed = 1.0f;}
    else if (ball_pos.x > 4.65f){std::cout<<"p2 defeated" << std::endl;ball_speed = 1.0f;}
    
    if (ball_pos.x < -4.9f || ball_pos.x > 4.9f){
        ball_move = glm::vec3(0);// end round
    }
        
    if (glm::length(ball_move) > 1.0f){//restrain speed
        ball_move = glm::normalize(ball_move);
    }
    
    p1_pos += p1_move*player_speed*deltaTime;
    p2_pos += p2_move*player_speed*deltaTime;
    ball_pos += ball_move*ball_speed*deltaTime;
    
    p1 = glm::translate(p1, p1_pos);
    p2 = glm::translate(p2, p2_pos);
    ball = glm::translate(ball, ball_pos);
    std::cout<<"ball_pos x: " << ball_pos.x << std::endl;
    std::cout<<"ball_pos y: " << ball_pos.y << std::endl;
    
    
}

void Render(){
    glClear(GL_COLOR_BUFFER_BIT);
    
    float paddle_v[] = {-0.15f,0.6f,-0.15f,-0.6f,0.15f,-0.6f,0.15f,0.6f};
    float ball_v[] = {-0.1f,0.1f,-0.1f,-0.1f,0.1f,-0.1f,0.1f,0.1f};
    
    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, paddle_v);
    glEnableVertexAttribArray(program.positionAttribute);
    
    program.SetModelMatrix(p1);
    program.SetColor(0.396f, 0.706f, 0.8f, 1.0f);
    glDrawArrays(GL_QUADS, 0, 4);
    
    program.SetModelMatrix(p2);
    program.SetColor(0.831f, 0.314f, 0.149f, 1.0f);
    glDrawArrays(GL_QUADS, 0, 4);
    
    glDisableVertexAttribArray(program.positionAttribute);
    
    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, ball_v);
    glEnableVertexAttribArray(program.positionAttribute);
    
    program.SetModelMatrix(ball);
    program.SetColor(0.2f, 0.2f, 0.2f, 1.0f);
    glDrawArrays(GL_QUADS, 0, 4);
    
    glDisableVertexAttribArray(program.positionAttribute);
    
    
    SDL_GL_SwapWindow(displayWindow);
    
}

int main(int argc, char* argv[]) {
    
    Initialize();
    
    while (gameIsRunning) {
        
        ProcessInput();
        Update();
        Render();
    }
    
    SDL_Quit();
    return 0;
}
