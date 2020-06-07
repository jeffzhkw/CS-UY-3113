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
glm::mat4 viewMatrix, modelMatrix, projectionMatrix;

float player_x = 0;
float player_rotate = 0;

GLuint playerTextureID;

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

void Initialize() {
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("Textured triganle!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640 , 480, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
    
#ifdef _WINDOWS
    glewInit();
#endif
    
    //glViewport(0, 0, 640, 480);
    
    program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");//under shaders folder
    
    viewMatrix = glm::mat4(1.0f);
    modelMatrix = glm::mat4(1.0f);
    projectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);//orthographic projection:
    //looking from -5 to 5 horizontal -3.75 to 3.75 vertical: the box of the world I see
    
    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);
    //program.SetColor(1.0f, 0.0f, 0.0f, 1.0f);// set the color to red
    
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);//set background color: whenever clear the window, use this color
    
    
    glUseProgram(program.programID);
    playerTextureID = LoadTexture("ctg.png");
    
}

void ProcessInput() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
            gameIsRunning = false;
        }
    }
}

float lastTicks = 0.0f;
void Update() {
    float ticks = (float)SDL_GetTicks() /1000.0f;
    //give you the amount of ms since SDL is initialized
    float deltaTime = ticks - lastTicks;
    lastTicks = ticks;
    
    player_x += 1.0f * deltaTime;
    player_rotate += 90.0f * deltaTime;
    
    
    modelMatrix = glm::mat4(1.0f);
    //modelMatrix = glm::rotate(modelMatrix, glm::radians(player_rotate),glm::vec3(0.0f,0.0f,1.0f));
    //the order matters here.
    modelMatrix = glm::translate(modelMatrix, glm::vec3(player_x,0.0f,0.0f)); //z is 0.o
    modelMatrix = glm::rotate(modelMatrix, glm::radians(player_rotate),glm::vec3(0.0f,0.0f,1.0f));
    
    //modelMatrix = glm::rotate(modelMatrix, glm::radians(1.0f), glm::vec3(0.0f,0.0f,1.0f));
    
    //modelMatrix = glm::scale(modelMatrix, glm::vec3(1.01f,1.02f,1.0f));//z is 1.0
    
    
}

void Render() {
    
    glClear(GL_COLOR_BUFFER_BIT);
    
    float vertices[] = {-0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5};
    float texCoords[] = {0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0};
    
    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    
    glEnableVertexAttribArray(program.positionAttribute);
    glEnableVertexAttribArray(program.texCoordAttribute);
    
    program.SetModelMatrix(modelMatrix);
    glBindTexture(GL_TEXTURE_2D, playerTextureID);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    glDisableVertexAttribArray(program.positionAttribute);
    glDisableVertexAttribArray(program.texCoordAttribute);
    
    SDL_GL_SwapWindow(displayWindow);// take what's in the memory to the screen;
    
    
    /*glClear(GL_COLOR_BUFFER_BIT);//do clear using the color set before
    
    program.SetModelMatrix(modelMatrix);//tell the shader program to draw the next thing
    //line 35: modelMatrix = glm::mat4(1.0f);
    
    float vertices[] = { 0.5f, -0.5f, 0.0f, 0.5f, -0.5f, -0.5f };
    //x0,y0,x1,y1,x2,y2
    //apply matrix model to all three vertices, then draw it.
    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program.positionAttribute);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glDisableVertexAttribArray(program.positionAttribute);
    
    SDL_GL_SwapWindow(displayWindow);*/
}

void Shutdown() {
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    
    Initialize();
    
    while (gameIsRunning) {
        ProcessInput();
        Update();
        Render();
    }
    
    Shutdown();
    return 0;
}
