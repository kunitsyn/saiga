#pragma once

#include "saiga/camera/controllable_camera.h"
#include <GLFW/glfw3.h>
#include <saiga/glfw/glfw_eventhandler.h>

template<typename camera_t>
class Glfw_Camera : public Controllable_Camera<camera_t>{
public:
    Glfw_Camera(){
        this->keyboardmap = {
            GLFW_KEY_UP,
            GLFW_KEY_DOWN,
            GLFW_KEY_LEFT,
            GLFW_KEY_RIGHT,
            GLFW_KEY_LEFT_SHIFT,
            GLFW_KEY_SPACE
        };
        this->mousemap = {
            GLFW_MOUSE_BUTTON_3
        };
    }
};


