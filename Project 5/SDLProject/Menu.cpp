//
//  Menu.cpp
//  Project5
//
//  Created by Kewei Zhang on 7/21/20.
//  Copyright © 2020 ctg. All rights reserved.
//

#include "Menu.h"
GLuint fontTextureID;

void Menu::Initialize(){
    //initialize fonts
    state.player = new Entity();
    state.nextScene = -1;
    fontTextureID = Util::LoadTexture("font.png");
   
}

void Menu::Update(float deltaTime){
    
    const Uint8 *keys = SDL_GetKeyboardState(NULL);

    if (keys[SDL_SCANCODE_RETURN]) {
        state.nextScene = 1;
    }
    
}

void Menu::Render(ShaderProgram *program){
    Util::DrawText(program, fontTextureID, "An Untitled Platformer", 0.5, -0.25, glm::vec3(-3,0,0));
    Util::DrawText(program, fontTextureID, "press ENTER to play!", 0.5, -0.25, glm::vec3(-2.5,-1,0));
}
