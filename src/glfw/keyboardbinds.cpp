/**
 * Copyright (c) 2017 Darius Rückert 
 * Licensed under the MIT License.
 * See LICENSE file for more information.
 */

#include "saiga/glfw/keyboardbinds.h"

#include <saiga/opengl/opengl.h>
#include <GLFW/glfw3.h>

namespace Saiga {

KeyboardBinds::KeyboardBinds(){
    glfw_EventHandler::addKeyListener(this,20);

    IC.add("bind", [this](ICPARAMS){
        nextCommand = args;
        //todo check if command valid
        *(args.os)<<"Press any key to bind the commad '"<<nextCommand.args<<"'"<<std::endl;
        this->waitingForKey = true;
    });
}

bool KeyboardBinds::key_event(GLFWwindow* window, int key, int scancode, int action, int mods){
    (void)window;(void)scancode;(void)mods;
    if(waitingForKey){
        if(action!=GLFW_PRESS)
            return true;

        auto it = keyMap.find(key);
        if(it!=keyMap.end()){
            *(nextCommand.os)<<"KeyboardBinds: key already bound to '"<<(it->second)<<"'"<<std::endl;
        }else{
            keyMap.insert(mapElement(key,nextCommand.args));
            *(nextCommand.os)<<"Key Bind added: ("<<key<<" "<<nextCommand.args<<")"<<std::endl;
        }
        waitingForKey = false;
        return true;
    }else{
        auto it = keyMap.find(key);
        if(it!=keyMap.end()){
            if(action!=GLFW_PRESS)
                return true;
            IC.execute(it->second);
            return true;
        }
    }


    return false;
}

bool KeyboardBinds::character_event(GLFWwindow* window, unsigned int codepoint){
    (void)window;(void)codepoint;
    return false;
}

}
