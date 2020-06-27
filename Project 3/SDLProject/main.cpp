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


#define BLOCK_COUNT 50
#define MENU_LETTER_COUNT 20
#define WIN_LETTER_COUNT 15
#define LOSE_LETTER_COUNT 9
struct GameState{
    Entity *player;
    Entity *blocks;
    Entity *menuWords;
    Entity *winWords;
    Entity *loseWords;
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
        displayWindow = SDL_CreateWindow("Lunar Landing by Kewei Zhang", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 768, 576, SDL_WINDOW_OPENGL);
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
    
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    //---------player-----------//
    state.player = new Entity();
    state.player->position = glm::vec3(3.5f,5.0f,0.0f);
    state.player->acceleration = glm::vec3(0.0f,-0.4f,0.0f);
    state.player->speed = 1.0f;
    state.player->textureID = LoadTexture("ship.png");//To-do: width/height check
    state.player->entityType = PLAYER;
    
    //---------blocks-----------//
    state.blocks = new Entity[BLOCK_COUNT];
    GLuint stoneID = LoadTexture("stone.png");
    GLuint dirtID = LoadTexture("dirt.png");
    GLuint grassID = LoadTexture("grass.png");
    for (int i = 0; i < 8; i++){
        state.blocks[i].textureID = stoneID;
        state.blocks[i].position = glm::vec3(-5.5f, (float)4-i, 0.0f);
    }
    state.blocks[8].textureID = dirtID;
    state.blocks[8].position = glm::vec3(-5.5f, -4.0f,0.0f);
    state.blocks[9].textureID = dirtID;
    state.blocks[9].position = glm::vec3(-4.5f, -4.0f,0.0f);
    state.blocks[11].textureID = dirtID;
    state.blocks[11].position = glm::vec3(-3.5f, -4.0f,0.0f);
    state.blocks[19].textureID = dirtID;
    state.blocks[19].position = glm::vec3(-0.5f, -4.0f,0.0f);
    state.blocks[21].textureID = dirtID;
    state.blocks[21].position = glm::vec3(0.5f, -4.0f,0.0f);
    state.blocks[23].textureID = dirtID;
    state.blocks[23].position = glm::vec3(1.5f, -4.0f,0.0f);
    state.blocks[26].textureID = dirtID;
    state.blocks[26].position = glm::vec3(2.5f, -4.0f,0.0f);
    state.blocks[29].textureID = dirtID;
    state.blocks[29].position = glm::vec3(3.5f, -4.0f,0.0f);
    state.blocks[32].textureID = dirtID;
    state.blocks[32].position = glm::vec3(4.5f, -4.0f,0.0f);
    state.blocks[41].textureID = dirtID;
    state.blocks[41].position = glm::vec3(5.5f, -4.0f,0.0f);
    state.blocks[15].textureID = grassID;
    state.blocks[15].position = glm::vec3(-2.5f, -4.0f,0.0f);
    state.blocks[17].textureID = grassID;
    state.blocks[17].position = glm::vec3(-1.5f, -4.0f,0.0f);
    
    state.blocks[10].textureID = stoneID;
    state.blocks[10].position = glm::vec3(-3.5f,1.0f,0.0f);
    state.blocks[12].textureID = stoneID;
    state.blocks[12].position = glm::vec3(-2.5f,2.0f,0.0f);
    state.blocks[13].textureID = stoneID;
    state.blocks[13].position = glm::vec3(-2.5f,1.0f,0.0f);
    state.blocks[14].textureID = stoneID;
    state.blocks[14].position = glm::vec3(-2.5f,0.0f,0.0f);
    state.blocks[16].textureID = stoneID;
    state.blocks[16].position = glm::vec3(-1.5f,0.0f,0.0f);
    state.blocks[18].textureID = stoneID;
    state.blocks[18].position = glm::vec3(-0.5f,-0.5f,0.0f);
    state.blocks[20].textureID = stoneID;
    state.blocks[20].position = glm::vec3(0.5f,2.0f,0.0f);
    state.blocks[22].textureID = stoneID;
    state.blocks[22].position = glm::vec3(1.5f,2.0f,0.0f);
    state.blocks[24].textureID = stoneID;
    state.blocks[24].position = glm::vec3(2.5f,3.0f,0.0f);
    state.blocks[25].textureID = stoneID;
    state.blocks[25].position = glm::vec3(2.5f,2.0f,0.0f);
    state.blocks[27].textureID = stoneID;
    state.blocks[27].position = glm::vec3(3.5f,3.0f,0.0f);
    state.blocks[28].textureID = stoneID;
    state.blocks[28].position = glm::vec3(3.5f,-1.0f,0.0f);
    state.blocks[30].textureID = stoneID;
    state.blocks[30].position = glm::vec3(4.5f,3.0f,0.0f);
    state.blocks[31].textureID = stoneID;
    state.blocks[31].position = glm::vec3(4.5f,-1.0f,0.0f);
    
    for (int i = 33; i < 41; i++){
        state.blocks[i].textureID = stoneID;
        state.blocks[i].position = glm::vec3(5.5f, (float)37-i, 0.0f);
    }
    for (int i = 0; i < 41; i++){
        if (i == 15 || i == 17) state.blocks[i].entityType = TARGET;
        else state.blocks[i].entityType = REG;
    }
    
    //---------spikes-----------//
    GLuint spikeID = LoadTexture("spike.png");
    for (int i = 42; i < BLOCK_COUNT; i++){
        state.blocks[i].textureID = spikeID;
        state.blocks[i].height = 0.5;
        state.blocks[i].entityType = HARM;
        if (i < 2+42){
            state.blocks[i].position = glm::vec3((float)i-4.5-42, -3.0f, 0.0f);
        }
        else{
            state.blocks[i].position = glm::vec3((float)i-2.5-42, -3.0f, 0.0f);
        }
    }
    
    GLuint fontID = LoadTexture("font.png");
    //---------menu-----------//
    state.menuWords = new Entity[MENU_LETTER_COUNT];
    for (int i = 0; i < MENU_LETTER_COUNT; i++){
        state.menuWords[i].textureID = fontID;
        state.menuWords[i].position = glm::vec3((float)(-4.5f+(i/2.0f)),-1.0f,0.0f);
    }
    state.menuWords[0].fontIndex = 80;
    state.menuWords[1].fontIndex = 114;
    state.menuWords[2].fontIndex = 101;
    state.menuWords[3].fontIndex = 115;
    state.menuWords[4].fontIndex = 115;
    state.menuWords[5].fontIndex = 32;
    state.menuWords[6].fontIndex = 83;
    state.menuWords[7].fontIndex = 80;
    state.menuWords[8].fontIndex = 65;
    state.menuWords[9].fontIndex = 67;
    state.menuWords[10].fontIndex = 69;
    state.menuWords[11].fontIndex = 32;
    state.menuWords[12].fontIndex = 116;
    state.menuWords[13].fontIndex = 111;
    state.menuWords[14].fontIndex = 32;
    state.menuWords[15].fontIndex = 115;
    state.menuWords[16].fontIndex = 116;
    state.menuWords[17].fontIndex = 97;
    state.menuWords[18].fontIndex = 114;
    state.menuWords[19].fontIndex = 116;
    //---------win-----------//
    state.winWords = new Entity[WIN_LETTER_COUNT];
    for (int i = 0; i < WIN_LETTER_COUNT; i++){
        state.winWords[i].textureID = fontID;
        state.winWords[i].position = glm::vec3((float)(-4.5f+(i/2.0f)),1.0f,0.0f);
    }
    state.winWords[0].fontIndex = 77;
    state.winWords[1].fontIndex = 73;
    state.winWords[2].fontIndex = 83;
    state.winWords[3].fontIndex = 83;
    state.winWords[4].fontIndex = 73;
    state.winWords[5].fontIndex = 79;
    state.winWords[6].fontIndex = 78;
    state.winWords[7].fontIndex = 32;
    state.winWords[8].fontIndex = 83;
    state.winWords[9].fontIndex = 85;
    state.winWords[10].fontIndex = 67;
    state.winWords[11].fontIndex = 67;
    state.winWords[12].fontIndex = 69;
    state.winWords[13].fontIndex = 83;
    state.winWords[14].fontIndex = 83;
    //---------lose-----------//
    state.loseWords = new Entity[LOSE_LETTER_COUNT];
    for (int i = 0; i < LOSE_LETTER_COUNT; i++){
        state.loseWords[i].textureID = fontID;
        state.loseWords[i].position = glm::vec3((float)(-4.5f+(i/2.0f)),-1.0f,0.0f);
    }
    state.loseWords[0].fontIndex = 71;
    state.loseWords[1].fontIndex = 65;
    state.loseWords[2].fontIndex = 77;
    state.loseWords[3].fontIndex = 69;
    state.loseWords[4].fontIndex = 32;
    state.loseWords[5].fontIndex = 79;
    state.loseWords[6].fontIndex = 86;
    state.loseWords[7].fontIndex = 69;
    state.loseWords[8].fontIndex = 82;
    currMode = END;
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
                        
                        std::cout << "in" <<currMode<<"pressed" << std::endl;
                        state.player->position = glm::vec3(3.5f,5.0f,0.0f);
                        state.player->lastCollide = PLAYER;
                        currMode = LEVEL;
                        break;
                }
                break;//end of SDL_KEYDOWN
        }
    }
}

void ProcessLevel(){
    state.player->acceleration = glm::vec3(0.0f,-0.4f,0.0f);
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
        state.player->acceleration.x = -0.5f;
    }
    else if (keys[SDL_SCANCODE_RIGHT]) {
        state.player->acceleration.x = 0.5f;
    }
    if (keys[SDL_SCANCODE_SPACE]){
        state.player->acceleration.y = 0.8f;
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
                        
                        std::cout << "in" << currMode <<"pressed" << std::endl;
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


#define FIXED_TIMESTEP 0.0166666f
float lastTicks = 0;
float accumulator = 0.0f;
void UpdateMain(float deltaTime){
    for (int i = 0; i < MENU_LETTER_COUNT; i++){
        state.menuWords[i].Update(0, NULL, 0);
    }
}
void UpdateLevel(float deltaTime){
    for (int i =0; i < BLOCK_COUNT; i++){
        state.blocks[i].Update(0, NULL, 0);
    }
    state.player->Update(FIXED_TIMESTEP, state.blocks, BLOCK_COUNT);
    
    if (state.player->lastCollide == HARM||state.player->lastCollide == TARGET){
        currMode = END;
    }
    //check for game over, currState = END
}
void UpdateEnd(float deltaTime){
    if (state.player->lastCollide == HARM){
        //mission fail
        for (int i = 0; i < LOSE_LETTER_COUNT; i++){
           state.loseWords[i].Update(0, NULL, 0);
       }
    }
    else if (state.player->lastCollide == TARGET){
        //mission success!
        for (int i = 0; i < WIN_LETTER_COUNT; i++){
            state.winWords[i].Update(0, NULL, 0);
        }
        
    }
}

void Update(){
    float ticks = (float)SDL_GetTicks() /1000.0f;
    float deltaTime = ticks - lastTicks;
    lastTicks = ticks;
    
    deltaTime += accumulator;
    if (deltaTime < FIXED_TIMESTEP) {
        accumulator = deltaTime;
        return;
    }
    
    while (deltaTime >= FIXED_TIMESTEP){
        switch (currMode){
            case MAIN_MENU:
                UpdateMain(deltaTime);
                break;
            case LEVEL:
                //Todo: call .Update() for each block
                UpdateLevel(deltaTime);
                break;
            case END:
                UpdateEnd(deltaTime);
                break;
        }
        deltaTime -= FIXED_TIMESTEP;
        
    }
    accumulator = deltaTime;
}
void RenderMain(){
    for (int i = 0; i < MENU_LETTER_COUNT; i++){
        state.menuWords[i].Render(&program);
    }
}
void RenderLevel(){
    for (int i =0; i < BLOCK_COUNT; i++){
        state.blocks[i].Render(&program);
    }

    state.player->Render(&program);
    std::cout << state.player->velocity.x << std::endl;
    std::cout << state.player->velocity.y << std::endl;
}
void RenderEnd(){
    if (state.player->lastCollide == HARM){
        //mission fail
        for (int i = 0; i < LOSE_LETTER_COUNT; i++){
           state.loseWords[i].Render(&program);
       }
    }
    else if (state.player->lastCollide == TARGET){
        //mission success!
        for (int i = 0; i < WIN_LETTER_COUNT; i++){
            state.winWords[i].Render(&program);
        }
        
    }
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
