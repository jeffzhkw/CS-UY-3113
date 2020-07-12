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
#include "stb_image.h"
#include "Entity.h"

#define BLOCK_COUNT 77
#define ENEMY_COUNT 3
#define WIN_LETTER_COUNT 15
#define LOSE_LETTER_COUNT 9
struct GameState {
    Entity *player;
    Entity *blocks;
    Entity *enemies;
    Entity *winWords;
    Entity *loseWords;
};

GameState state;

SDL_Window* displayWindow;
bool gameIsRunning = true;


ShaderProgram program;
glm::mat4 viewMatrix, modelMatrix, projectionMatrix;

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
        displayWindow = SDL_CreateWindow("Hello, World!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1024, 768, SDL_WINDOW_OPENGL);
        SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
        SDL_GL_MakeCurrent(displayWindow, context);
        
    #ifdef _WINDOWS
        glewInit();
    #endif
    
    program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");
    
    viewMatrix = glm::mat4(1.0f);
    modelMatrix = glm::mat4(1.0f);
    projectionMatrix = glm::ortho(-8.0f, 8.0f, -6.0f, 6.0f, -1.0f, 1.0f);
    
    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);
    
    glUseProgram(program.programID);
    
    glClearColor(0.8f, 0.8f, 0.8f, 0.8f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    //Initialize game entity
    
    //player
    
    state.player = new Entity();
    state.player->entityType = PLAYER;
    state.player->height = 0.8f;
    state.player->width = 0.8f;
    state.player->position = glm::vec3(5.5,5.5,0);
    state.player->movement = glm::vec3(0);
    state.player->acceleration = glm::vec3(0, -9.82f, 0);
    state.player->speed = 3.0f;
    state.player->jumpPower = 10.0f;
    state.player->textureID = LoadTexture("player_left.png");
    
    //blocks
    state.blocks = new Entity[BLOCK_COUNT];
    GLuint blockTextureID = LoadTexture("block.png");
    
    for (int i = 0; i < 16; i++){
        state.blocks[i].entityType = BLOCK;
        state.blocks[i].textureID = blockTextureID;
        state.blocks[i].position = glm::vec3(-7.5+i,-5.5f,0);
    }
    for (int i = 16; i < 27; i++){
        state.blocks[i].entityType = BLOCK;
        state.blocks[i].textureID = blockTextureID;
        state.blocks[i].position = glm::vec3(-7.5,5.5+16-i,0);
    }
    for (int i = 27; i < 38; i++){
        state.blocks[i].entityType = BLOCK;
        state.blocks[i].textureID = blockTextureID;
        state.blocks[i].position = glm::vec3(7.5,5.5+27-i,0);
    }
    for (int i = 38; i < 50; i++){
        state.blocks[i].entityType = BLOCK;
        state.blocks[i].textureID = blockTextureID;
        state.blocks[i].position = glm::vec3(-6.5-38+i,5.5,0);
    }
    state.blocks[50].entityType = BLOCK;
    state.blocks[50].textureID = blockTextureID;
    state.blocks[50].position = glm::vec3(6.5,5.5,0);
    
    for (int i = 51; i < 63; i++){
        state.blocks[i].entityType = BLOCK;
        state.blocks[i].textureID = blockTextureID;
        state.blocks[i].position = glm::vec3(-5.5-51+i,-1.5,0);
    }
    state.blocks[63].entityType = BLOCK;
    state.blocks[63].textureID = blockTextureID;
    state.blocks[63].position = glm::vec3(-3.5,3.5,0);
    
    state.blocks[64].entityType = BLOCK;
    state.blocks[64].textureID = blockTextureID;
    state.blocks[64].position = glm::vec3(-3.5,2.5,0);
    
    state.blocks[65].entityType = BLOCK;
    state.blocks[65].textureID = blockTextureID;
    state.blocks[65].position = glm::vec3(4.5,2.5,0);
    state.blocks[66].entityType = BLOCK;
    state.blocks[66].textureID = blockTextureID;
    state.blocks[66].position = glm::vec3(5.5,2.5,0);
    state.blocks[67].entityType = BLOCK;
    state.blocks[67].textureID = blockTextureID;
    state.blocks[67].position = glm::vec3(6.5,2.5,0);
    for (int i = 68; i < BLOCK_COUNT; i++){
        state.blocks[i].entityType = BLOCK;
        state.blocks[i].textureID = blockTextureID;
        state.blocks[i].position = glm::vec3(-3.5-68+i,1.5,0);
    }
    
    for (int i =0; i < BLOCK_COUNT; i++){
        state.blocks[i].Update(0, NULL, NULL, 0);
    }
    
    //Enemey
    state.enemies = new Entity[ENEMY_COUNT];
    for (int i = 0; i < ENEMY_COUNT; i++){
        state.enemies[i].entityType = ENEMY;
        state.enemies[i].height = 1.0f;
        state.enemies[i].width = 1.0f;
        state.enemies[i].movement = glm::vec3(0);
        state.enemies[i].acceleration = glm::vec3(0, -9.82f, 0);
        state.enemies[i].speed = 1.0f;
        state.enemies[i].jumpPower = 1.0f;
        state.enemies[i].textureID = LoadTexture("enemy_left.png");
        state.enemies[i].aiState = IDLE;
    }
    state.enemies[0].position = glm::vec3(1.5,2.5,0);
    state.enemies[0].aiType = WALK;
    state.enemies[1].position = glm::vec3(0.5,-0.5,0);
    state.enemies[1].aiType = TRACK;
    state.enemies[2].position = glm::vec3(-3.5,-4.5,0);
    state.enemies[2].aiType = DASH;
    
    GLuint fontID = LoadTexture("font.png");
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
    for (int i = 0; i < WIN_LETTER_COUNT; i++){
               state.winWords[i].Update(0, NULL, NULL, 0);
    }
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
    for (int i = 0; i < LOSE_LETTER_COUNT; i++){
        state.loseWords[i].Update(0, NULL, NULL, 0);
    }
}

void ProcessInput(){
   state.player->movement = glm::vec3(0);
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
            case SDL_WINDOWEVENT_CLOSE:
                gameIsRunning = false;
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_0:
                        state.player->isActive = false;
                        break;
                    case SDLK_9:
                        state.enemies[0].isActive = false;
                        break;
                    case SDLK_8:
                        state.enemies[1].isActive = false;
                        break;
                    case SDLK_7:
                        state.enemies[2].isActive = false;
                        break;
                    case SDLK_SPACE:
                        // Some sort of action
                        if (state.player->collidedBottom){
                            state.player->jump = true;
                        }
                        break;
                }
                break; // SDL_KEYDOWN
        }
    }
    const Uint8 *keys = SDL_GetKeyboardState(NULL);
    if (keys[SDL_SCANCODE_LEFT]) {
        state.player->movement.x = -1.0f;
        state.player->animIndices = state.player->animLeft;
        state.player->textureID = LoadTexture("player_left.png");
    }
    else if (keys[SDL_SCANCODE_RIGHT]) {
        state.player->movement.x = 1.0f;
        state.player->animIndices = state.player->animRight;
        state.player->textureID = LoadTexture("player_right.png");
    }
    if (glm::length(state.player->movement) > 1.0f) {
        state.player->movement = glm::normalize(state.player->movement);
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
        // Update. Notice it's FIXED_TIMESTEP. Not deltaTime
        state.player->Update(FIXED_TIMESTEP, state.player, state.blocks, BLOCK_COUNT);
        for (int i =0; i < ENEMY_COUNT; i++){
            if(state.enemies[i].movement.x > 0){
                state.enemies[i].textureID = LoadTexture("enemy_right.png");
            }
            else if (state.enemies[i].movement.x < 0){
                state.enemies[i].textureID = LoadTexture("enemy_left.png");
            }
            state.enemies[i].Update(FIXED_TIMESTEP, state.player, state.blocks, BLOCK_COUNT);
        }
        deltaTime -= FIXED_TIMESTEP;
    }
    accumulator = deltaTime;
    
}
void Render(){
    glClear(GL_COLOR_BUFFER_BIT);
    
    for (int i =0; i < BLOCK_COUNT; i++){
        state.blocks[i].Render(&program);
    }

    for (int i = 0; i < ENEMY_COUNT; i++){
        state.enemies[i].Render(&program);
    }
    state.player->Render(&program);
    
    if (state.player->isActive == false){
        for (int i = 0; i < LOSE_LETTER_COUNT; i++){
            state.loseWords[i].Render(&program);
        }
    }
    if (state.enemies[0].isActive == false && state.enemies[1].isActive == false && state.enemies[2].isActive == false){
        for (int i = 0; i < WIN_LETTER_COUNT; i++){
            state.winWords[i].Render(&program);
        }
    }

    SDL_GL_SwapWindow(displayWindow);
    
}
int main(int argc, char* argv[]) {
    Initialize();
    while(gameIsRunning){
        ProcessInput();
        Update();
        Render();
    }
    SDL_Quit();
    return 0;
}
