/**
 * Copyright (c) 2017 Darius Rückert 
 * Licensed under the MIT License.
 * See LICENSE file for more information.
 */

#include "saiga/rendering/object3d.h"

namespace Saiga {

void Object3D::setSimpleDirection(const vec3 &dir){
    this->rot = getSimpleDirectionQuat(dir);
}



void Object3D::turn(float angleX, float angleY){
    rotateGlobal(vec3(0,1,0),angleX);
    mat4 modeltmp = model;
    rotateLocal(vec3(1,0,0),angleY);
    if (model[1][1] < 0){
        model = modeltmp;
    }
}

void Object3D::turnLocal(float angleX, float angleY)
{
    rotateLocal(vec3(0,1,0),angleX);
    rotateLocal(vec3(1,0,0),angleY);
}


void Object3D::rotateAroundPoint(const vec3& point, const vec3& axis, float angle){
    rotateLocal(axis,angle);

    translateGlobal(-point);
    quat qrot = glm::angleAxis(glm::radians(angle),axis);
//    position = vec3(qrot*vec4(position,1));
    position = qrot*position;
    translateGlobal(point);
}

quat Object3D::getSimpleDirectionQuat(const vec3 &dir){
    mat4 rotmat(1);
    rotmat[0] = vec4(glm::normalize(glm::cross(dir,vec3(0,1,0))),0);
    rotmat[1] = vec4(0,1,0,0);
    rotmat[2] = vec4(-dir,0);

    return glm::normalize(quat(rotmat));
}

}
