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

SDL_Window* displayWindow;
bool gameIsRunning = true;

ShaderProgram program;
glm::mat4 viewMatrix, projectionMatrix;


glm::mat4 sandGlassMat;
glm::mat4 emoMat;
GLuint sandGlassTexID;
GLuint emoTexID;


GLuint LoadTexture(const char* filePath){//loading image to texture
    int w, h, n;
    unsigned char* image = stbi_load(filePath, &w, &h, &n, STBI_rgb_alpha);//raw pixel data
    if (image == NULL){
        std::cout << "Unable to load the image. Make sure the path is correct.\n";
        assert(false);
    }
    
    GLuint textureID;
    glGenTextures(1, &textureID);
    
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
    return textureID;
    
}

void initialize(){
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("Project 1 by Kewei Zhang", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
        
    #ifdef _WINDOWS
        glewInit();
    #endif
    
    program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");
    
    viewMatrix = glm::mat4(1.0f);
    projectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);//orthographic projection
    program.SetViewMatrix(viewMatrix);
    program.SetProjectionMatrix(projectionMatrix);
    
    sandGlassMat = glm::mat4(1.0f);
    emoMat = glm::mat4(1.0f);
    
    
    //glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClearColor(0.945f, 0.863f, 0.612f, 1.0f);//set background color
    glEnable(GL_BLEND);
    glBlendFunc(GL_ZERO, GL_SRC_COLOR);
    
    
    sandGlassTexID = LoadTexture("p1.png");
    emoTexID = LoadTexture("p2.jpg");
    glUseProgram(program.programID);
    
    
}

float lastTick = 0.0f;

float angle = 0;
float emo_x = 0.0f;
void update(){
    float ticks = (float)SDL_GetTicks() /1000.0f;
    float deltaTime = ticks - lastTick;
    lastTick = ticks;
    
    angle = angle + 90.0f*deltaTime;
    emo_x = emo_x + 1.0f*deltaTime;
    
    //update object info here
    
    sandGlassMat = glm::mat4(1.0f);
    emoMat = glm::mat4(1.0f);
    
    sandGlassMat = glm::translate(sandGlassMat, glm::vec3(-4.2f,3.0f,0.0f));
    sandGlassMat = glm::scale(sandGlassMat, glm::vec3(2.5f,2.5f,1.0f));
    sandGlassMat = glm::rotate(sandGlassMat, glm::radians(angle), glm::vec3(0.0f,0.0f,1.0f));
    emoMat = glm::translate(emoMat, glm::vec3(emo_x, 0.0f, 0.0f));
    
    
}

void render(){
    glClear(GL_COLOR_BUFFER_BIT);
    
    float vertices[] = {-0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5};
    float texCoords[] = {0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0};
    
    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program.positionAttribute);
    glEnableVertexAttribArray(program.texCoordAttribute);
    
    //draw all my objects here
    
    program.SetModelMatrix(emoMat);
    glBindTexture(GL_TEXTURE_2D, emoTexID);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    program.SetModelMatrix(sandGlassMat);
    glBindTexture(GL_TEXTURE_2D, sandGlassTexID);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    
    
    
    glDisableVertexAttribArray(program.positionAttribute);
    glDisableVertexAttribArray(program.texCoordAttribute);
    
    
    
    
    SDL_GL_SwapWindow(displayWindow);
    
}

int main(int argc, char* argv[]) {
    
    
    initialize();
    
    while (gameIsRunning) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
                gameIsRunning = false;
            }
        }
        update();
        render();
    }
    
    SDL_Quit();
    return 0;
}
