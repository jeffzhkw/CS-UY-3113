//
//  Menu.cpp
//  Project5
//
//  Created by Kewei Zhang on 7/21/20.
//  Copyright © 2020 ctg. All rights reserved.
//

#include "Menu.h"
#define TITLE_COUNT
#define DESCRIPTION_COUNT

void Menu::Initialize(){
    //initialize fonts
    state.player = new Entity();
    state.nextScene = -1;
   
}

void Menu::Update(float deltaTime){
    
    const Uint8 *keys = SDL_GetKeyboardState(NULL);

    if (keys[SDL_SCANCODE_RETURN]) {
        state.nextScene = 1;
    }
    
}

void Menu::Render(ShaderProgram *program){
    
}
