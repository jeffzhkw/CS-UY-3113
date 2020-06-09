#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1
#include <SDL.h>
#include <SDL_opengl.h>

SDL_Window* displayWindow;
bool gameIsRunning = true;

void initialize(){
    SDL_Init(SDL_INIT_VIDEO);
        displayWindow = SDL_CreateWindow("Hello, World!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
        SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
        SDL_GL_MakeCurrent(displayWindow, context);
        
    #ifdef _WINDOWS
        glewInit();
    #endif
    
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        
    
}


void update(){
    
    
}

void render(){
    glClear(GL_COLOR_BUFFER_BIT);
    
    
    SDL_GL_SwapWindow(displayWindow);
    
}

int main(int argc, char* argv[]) {
    
    SDL_Event event;
    while (gameIsRunning) {
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
