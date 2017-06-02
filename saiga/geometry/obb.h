#pragma once

#include <saiga/config.h>
#include <saiga/util/glm.h>

/**
 * Oriented Bounding Box
 */
class SAIGA_GLOBAL OBB
{
public:
    //center point
    vec3 center;
    //the column vectors represent the main axis
    //and their length is the positive half extend
    glm::mat3 orientationScale;

    void setOrientationScale(vec3 x, vec3 y, vec3 z);


    void fitToPoints(int axis, vec3* points, int count);

    void normalize();

};
