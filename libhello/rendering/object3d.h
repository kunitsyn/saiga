#pragma once

#include "libhello/util/glm.h"
#include <glm/gtc/quaternion.hpp>


//a moving object is an object that moves every game tick and collides with the world

class Object3D{
private:

public:
    mat4 model;


    //required for non uniform scaled rotations
    //TODO: extra class so uniform objects are faster
    glm::quat rot;
//    mat4 rotation;
    mat4 size;
    mat4 base;

    void calculateModel(){model = base*glm::mat4_cast(rot)*size;}

   vec4 getPosition(){return vec4(base[3]);} //returns global position
    vec4 getDirection(){return rot*vec4(0,0,1,0);} //returns looking direction
    vec4 getRightVector(){return rot*vec4(1,0,0,0);}
    vec4 getUpVector(){return rot*vec4(0,1,0,0);}
    void setSimpleDirection(vec3 dir); //sets looking direction to dir, up to (0,1,0) and right to cross(dir,up)

    void translateLocal(vec4 d);
    void translateGlobal(vec4 d);
    void translateLocal(vec3 d);
    void translateGlobal(vec3 d);

    void rotateLocal2(vec3 axis, float angle);
    void rotateLocal(vec3 axis, float angle); //rotate around local axis (this is much faster than rotateGlobal)
    void rotateGlobal(vec3 axis, float angle);


    void scale(vec3 s);
    vec3 getScale();
    void setScale(vec3 s);
    void normalize();

    virtual void setPosition(vec3 cords){base[3] = vec4(cords,1);}
    virtual void turn(float angleX, float angleY);
    virtual void getModelMatrix(mat4& m){m = model;}
    virtual void getViewMatrix(mat4& view); //the view matrix is the inverse model matrix


};

