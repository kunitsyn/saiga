/**
 * Copyright (c) 2017 Darius Rückert 
 * Licensed under the MIT License.
 * See LICENSE file for more information.
 */

#pragma once

#include "saiga/rendering/lighting/point_light.h"

namespace Saiga {

class SAIGA_GLOBAL SpotLightShader : public AttenuatedLightShader{
public:
    GLint location_angle;
    GLint location_shadowPlanes;
    virtual void checkUniforms();
    void uploadAngle(float angle);
    void uploadShadowPlanes(float f, float n);
};



class SAIGA_GLOBAL SpotLight :  public AttenuatedLight
{
    friend class DeferredLighting;
protected:
    float angle=60.0f;
    Shadowmap shadowmap;
public:
    float shadowNearPlane = 0.1f;
    PerspectiveCamera shadowCamera;

    /**
     * The default direction of the mesh is negative y
     */

    SpotLight();
    virtual ~SpotLight(){}
    void bindUniforms(std::shared_ptr<SpotLightShader> shader, Camera *shadowCamera);


    void setRadius(float value) override;

    void createShadowMap(int resX, int resY);

    void recalculateScale();
    void setAngle(float value);
    float getAngle() const{return angle;}

    void setDirection(vec3 dir);

    void calculateCamera();
    bool cullLight(Camera *shadowCamera);

    void renderImGui();
};

}