/**
 * Copyright (c) 2017 Darius Rückert 
 * Licensed under the MIT License.
 * See LICENSE file for more information.
 */

#pragma once
#include "saiga/glfw/glfw_eventhandler.h"
#include "saiga/util/inputcontroller.h"

namespace Saiga {

class SAIGA_GLOBAL KeyboardBinds: public glfw_KeyListener
{
private:
    bool waitingForKey = false;
    InputController::Operation::Arguments nextCommand;
    std::map<int,std::string> keyMap;
    typedef std::pair<int,std::string> mapElement;
public:
    KeyboardBinds();

    //glfw events
    bool key_event(GLFWwindow* window, int key, int scancode, int action, int mods);
    bool character_event(GLFWwindow* window, unsigned int codepoint);
};

}
