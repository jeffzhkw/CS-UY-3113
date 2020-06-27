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

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"//use for loading texture

#include "Entity.h"

ShaderProgram program;
SDL_Window* displayWindow;
bool gameIsRunning = true;

enum GameMode {MAIN_MENU, LEVEL, END};
GameMode currMode;
struct GameState{
    Entity *player;
    Entity *block;
    Entity *spike;
};
GameState state;

glm::mat4 viewMatrix, projectionMatrix;


GLuint LoadTexture(const char* filePath) {
    int w, h, n;
    unsigned char* image = stbi_load(filePath, &w, &h, &n, STBI_rgb_alpha);
    if (image == NULL) {
        std::cout << "Unable to load image. Make sure the path is correct\n";
        assert(false);
    }
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
    stbi_image_free(image);
    return textureID;
}

void Initialize(){
        SDL_Init(SDL_INIT_VIDEO);
        displayWindow = SDL_CreateWindow("Hello, World!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
        SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
        SDL_GL_MakeCurrent(displayWindow, context);
        
    #ifdef _WINDOWS
        glewInit();
    #endif
    
    program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");
    
    viewMatrix = glm::mat4(1.0f);
    projectionMatrix = glm::ortho(-6.0f, 6.0f, -4.5f, 4.5f, -1.0f, 1.0f);
    
    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);
    glUseProgram(program.programID);
    
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    currMode = LEVEL;
    
    
}

void ProcessMain(){
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
            case SDL_WINDOWEVENT_CLOSE:
                gameIsRunning = false;
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_SPACE:
                        currMode = LEVEL;
                        break;
                }
                break;//end of SDL_KEYDOWN
        }
    }
}

void ProcessLevel(){
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
            case SDL_WINDOWEVENT_CLOSE:
                gameIsRunning = false;
                break;
        }
    }
    const Uint8 *keys = SDL_GetKeyboardState(NULL);
    
    if (keys[SDL_SCANCODE_LEFT]) {
       
    }
    else if (keys[SDL_SCANCODE_RIGHT]) {
       
    }
    else if (keys[SDL_SCANCODE_SPACE]){
        
    }
    
    //Normalize
    if (glm::length(state.player->movement) > 1.0f) {
        state.player->movement = glm::normalize(state.player->movement);
    }
    
}

void ProcessEnd(){
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
            case SDL_WINDOWEVENT_CLOSE:
                gameIsRunning = false;
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_SPACE:
                        currMode = MAIN_MENU;
                        break;
                }
                break;
        }
    }
}
void ProcessInput(){
    switch(currMode){
        case MAIN_MENU:
            ProcessMain();
            break;
        case LEVEL:
            ProcessLevel();
            break;
        case END:
            ProcessEnd();
            break;
    }
}
void UpdateMain(float deltaTime){
    
}
void UpdateLevel(float deltaTime){
    
}
void UpdateEnd(float deltaTime){
    
}
float lastTicks = 0;
void Update(){
    float ticks = (float)SDL_GetTicks() /1000.0f;
    float deltaTime = ticks - lastTicks;
    lastTicks = ticks;
    
    switch (currMode){
        case MAIN_MENU:
            UpdateMain(deltaTime);
            break;
        case LEVEL:
            UpdateLevel(deltaTime);
            break;
        case END:
            UpdateEnd(deltaTime);
            break;
    }
}
void RenderMain(){
    
}
void RenderLevel(){
    
}
void RenderEnd(){
    
}
void Render(){
    glClear(GL_COLOR_BUFFER_BIT);
    switch (currMode){
        case MAIN_MENU:
            RenderMain();
            break;
        case LEVEL:
            RenderLevel();
            break;
        case END:
            RenderEnd();
            break;
    }
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
