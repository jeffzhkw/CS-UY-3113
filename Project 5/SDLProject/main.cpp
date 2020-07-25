#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_mixer.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"

SDL_Window* displayWindow;
bool gameIsRunning = true;

ShaderProgram program;
glm::mat4 viewMatrix, modelMatrix, projectionMatrix;

#include "Util.h"
#include "Entity.h"
#include "Map.h"
#include "Scene.h"
#include "Level1.h"
#include "Level2.h"
#include "Level3.h"
#include "Menu.h"


Scene *currentScene;
Scene *sceneList[4];
bool win;
void SwitchToScene(Scene *scene){
    currentScene = scene;
    currentScene->Initialize();
}
int lives;
Mix_Music *music;
Mix_Chunk *bounce;

void Initialize(){
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    displayWindow = SDL_CreateWindow("Hello, World!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
    
#ifdef _WINDOWS
    glewInit();
#endif
    program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");
    
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    
    music = Mix_LoadMUS("Post Malone, Swae Lee - Sunflower (Spider-Man Into the Spider-Verse).mp3");
    Mix_PlayMusic(music, -1);
    Mix_VolumeMusic(MIX_MAX_VOLUME /4 );
    
    bounce = Mix_LoadWAV("bounce.wav");
    viewMatrix = glm::mat4(1.0f);
    modelMatrix = glm::mat4(1.0f);
    projectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);
    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);
    
    glUseProgram(program.programID);
    
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    
    //initialize scenes
    sceneList[0] = new Menu();
    sceneList[1] = new Level1();
    sceneList[2] = new Level2();
    sceneList[3] = new Level3();
    SwitchToScene(sceneList[0]);
    lives = 3;
    win = false;
    
    
    
    
}

void ProcessInput(){
    
    //reset movement intention to zero
    currentScene->state.player->movement = glm::vec3(0);
    
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
            case SDL_WINDOWEVENT_CLOSE:
                gameIsRunning = false;
                break;
                
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_LEFT:
                        // Move the player left
                        break;
                        
                    case SDLK_RIGHT:
                        // Move the player right
                        break;
                        
                    case SDLK_SPACE:
                        // Some sort of action
                        if (currentScene->state.player->collidedBottom){
                            currentScene->state.player->jump = true;
                            Mix_PlayChannel(-1, bounce, 0);
                        }
                        break;
                }
                break; // SDL_KEYDOWN
        }
    }
    
    const Uint8 *keys = SDL_GetKeyboardState(NULL);

    if (keys[SDL_SCANCODE_LEFT]) {
        currentScene->state.player->movement.x = -1.0f;
        currentScene->state.player->animIndices = currentScene->state.player->animLeft;
    }
    else if (keys[SDL_SCANCODE_RIGHT]) {
        currentScene->state.player->movement.x = 1.0f;
               currentScene->state.player->animIndices = currentScene->state.player->animRight;
    }
    //normalize
    if (glm::length(currentScene->state.player->movement) > 1.0f) {
        currentScene->state.player->movement = glm::normalize(currentScene->state.player->movement);
    }

}

#define FIXED_TIMESTEP 0.0166666f
float lastTicks = 0;
float accumulator = 0.0f;
void Update(){
    float ticks = (float)SDL_GetTicks() / 1000.0f;
    float deltaTime = ticks - lastTicks;
    lastTicks = ticks;
    deltaTime += accumulator;
    
    if (deltaTime < FIXED_TIMESTEP) {
        accumulator = deltaTime;
        return;
    }
    
    while (deltaTime >= FIXED_TIMESTEP) {
        // Update curr scene. Notice it's FIXED_TIMESTEP. Not deltaTime
        currentScene->Update(FIXED_TIMESTEP);
        deltaTime -= FIXED_TIMESTEP;
    }
    accumulator = deltaTime;
    //update viewMatrix to follow the player
    viewMatrix= glm::mat4(1.0f);
    if (currentScene == sceneList[1]){
        if (currentScene->state.player->position.x < 5 && currentScene->state.player->position.y > -3.75){
            viewMatrix = glm::translate(viewMatrix, glm::vec3(-5, 3.75,0));
        }
        else if (currentScene->state.player->position.x > 16  && currentScene->state.player->position.y > -3.75){
            viewMatrix = glm::translate(viewMatrix, glm::vec3(-16, 3.75,0));
        }
        else if (currentScene->state.player->position.x > 16  && currentScene->state.player->position.y < -9.25){
            viewMatrix = glm::translate(viewMatrix, glm::vec3(-16, 9.25,0));
        }
        else if(currentScene->state.player->position.x < 5){
            viewMatrix = glm::translate(viewMatrix, glm::vec3(-5, -currentScene->state.player->position.y,0));
        }
        else if (currentScene->state.player->position.x > 16){
            viewMatrix = glm::translate(viewMatrix, glm::vec3(-16, -currentScene->state.player->position.y,0));
        }
        else if (currentScene->state.player->position.y > -3.75){
            viewMatrix = glm::translate(viewMatrix, glm::vec3(-currentScene->state.player->position.x, 3.75,0));
        }
        else if (currentScene->state.player->position.y < -9.25){
            viewMatrix = glm::translate(viewMatrix, glm::vec3(-currentScene->state.player->position.x, 9.25,0));
        }
        else{
            viewMatrix = glm::translate(viewMatrix, glm::vec3(-currentScene->state.player->position.x,-currentScene->state.player->position.y,0));
        }
        if (currentScene->state.player->position.y < -14){
            lives -= 1;
            currentScene->state.player->position = glm::vec3(5,-3.75,0);
        }
        
    }
    else if (currentScene == sceneList[2]){
        if (currentScene->state.player->position.x < 5 && currentScene->state.player->position.y > -3.75){
            viewMatrix = glm::translate(viewMatrix, glm::vec3(-5, 3.75,0));
        }
        else if (currentScene->state.player->position.x > 16  && currentScene->state.player->position.y > -3.75){
            viewMatrix = glm::translate(viewMatrix, glm::vec3(-16, 3.75,0));
        }
        else if (currentScene->state.player->position.x > 16  && currentScene->state.player->position.y < -4){
            viewMatrix = glm::translate(viewMatrix, glm::vec3(-16, 4,0));
        }
        else if (currentScene->state.player->position.x < 5 && currentScene->state.player->position.y < -4){
            viewMatrix = glm::translate(viewMatrix, glm::vec3(-5, 4,0));
        }
        else if(currentScene->state.player->position.x < 5){
            viewMatrix = glm::translate(viewMatrix, glm::vec3(-5, -currentScene->state.player->position.y,0));
        }
        else if (currentScene->state.player->position.x > 16){
            viewMatrix = glm::translate(viewMatrix, glm::vec3(-16, -currentScene->state.player->position.y,0));
        }
        else if (currentScene->state.player->position.y > -3.75){
            viewMatrix = glm::translate(viewMatrix, glm::vec3(-currentScene->state.player->position.x, 3.75,0));
        }
        else if (currentScene->state.player->position.y < -4){
            viewMatrix = glm::translate(viewMatrix, glm::vec3(-currentScene->state.player->position.x, 4,0));
        }
        else{
            viewMatrix = glm::translate(viewMatrix, glm::vec3(-currentScene->state.player->position.x,-currentScene->state.player->position.y,0));
        }
        
        if (currentScene->state.player->position.y < -8){
            lives -= 1;
            currentScene->state.player->position = glm::vec3(2,-3.75,0);
        }
        
    }
    else if (currentScene == sceneList[3]){
        if (currentScene->state.player->position.x < 5 && currentScene->state.player->position.y > -4){
            viewMatrix = glm::translate(viewMatrix, glm::vec3(-5, 4 ,0));
        }
        else if (currentScene->state.player->position.x > 21  && currentScene->state.player->position.y > -4){
            viewMatrix = glm::translate(viewMatrix, glm::vec3(-21, 4,0));
        }
        else if (currentScene->state.player->position.x > 21  && currentScene->state.player->position.y < -4){
            viewMatrix = glm::translate(viewMatrix, glm::vec3(-21, 4,0));
        }
        else if (currentScene->state.player->position.x < 5 && currentScene->state.player->position.y < -4){
            viewMatrix = glm::translate(viewMatrix, glm::vec3(-5, 4,0));
        }
        else if(currentScene->state.player->position.x < 5){
            viewMatrix = glm::translate(viewMatrix, glm::vec3(-5, currentScene->state.player->position.y,0));
        }
        else if (currentScene->state.player->position.x > 21){
            viewMatrix = glm::translate(viewMatrix, glm::vec3(-21, -currentScene->state.player->position.y,0));
        }
        else if (currentScene->state.player->position.y > -4){
            viewMatrix = glm::translate(viewMatrix, glm::vec3(-currentScene->state.player->position.x, 4,0));
        }
        else if (currentScene->state.player->position.y < -4){
            viewMatrix = glm::translate(viewMatrix, glm::vec3(-currentScene->state.player->position.x, 4,0));
        }
        else{
            viewMatrix = glm::translate(viewMatrix, glm::vec3(-currentScene->state.player->position.x,-currentScene->state.player->position.y,0));
        }
        if (currentScene->state.player->isActive == false){
            currentScene->state.player->isActive = true;
            lives -= 1;
            currentScene->state.player->position = glm::vec3(1,-7,0);
            currentScene->state.enemies->position = glm::vec3(12,-4,0);
            currentScene->state.enemies->velocity = glm::vec3(0);
        }
        
        if (currentScene->state.player->position.x >24.5 && currentScene->state.player->position.y < -5){
            win = true;
            std::cout << "win" << std::endl;
        }
        
    }
    std::cout << currentScene->state.player->position.x << std::endl;
    std::cout << currentScene->state.player->position.y << std::endl;

}

void Render(){
    glClear(GL_COLOR_BUFFER_BIT);
    
    
    program.SetViewMatrix(viewMatrix);
    
    if (lives <= 0 ){
        currentScene->state.player->isActive = false;
        Util::DrawText(&program, Util::LoadTexture("font.png"), "You Lose", 0.5, -0.25, glm::vec3(5,-3.75,0));
    }
    else if (win){
        Util::DrawText(&program, Util::LoadTexture("font.png"), "You Win!", 0.5, -0.25, glm::vec3(21,-3.75,0));
        currentScene->state.enemies->isActive = false;
    }
    currentScene->Render(&program);
    

    SDL_GL_SwapWindow(displayWindow);
}


int main(int argc, char* argv[]) {

    Initialize();
    
    while (gameIsRunning) {
        ProcessInput();
        Update();
        //scene switch
        if (currentScene->state.nextScene >= 0){
            SwitchToScene(sceneList[currentScene->state.nextScene]);
        }
        Render();
    }
    
    SDL_Quit();
    return 0;
}
