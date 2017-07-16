#pragma once

#include <saiga/config.h>
#include "saiga/util/glm.h"

namespace Saiga {


class SAIGA_GLOBAL Cone
{
public:
    vec3 position;
    vec3 direction = vec3(0,1,0);
    float radius = 1.0f;
    float height = 1.0f;


    Cone(void){}

    Cone(const vec3 &position, const vec3 &direction,float radius, float height)
        :position(position),direction(direction),radius(radius),height(height){}
    ~Cone(void){}



};

}
