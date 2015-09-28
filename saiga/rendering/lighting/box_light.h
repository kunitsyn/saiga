#pragma once


#include "saiga/rendering/lighting/directional_light.h"

class SAIGA_GLOBAL BoxLightShader : public DirectionalLightShader{
public:

    virtual void checkUniforms();

};

class SAIGA_GLOBAL BoxLight :  public DirectionalLight
{
protected:


public:
    BoxLight();
    virtual ~BoxLight(){}

    void bindUniforms(BoxLightShader& shader, Camera* cam);

    virtual void createShadowMap(int resX, int resY) override;
    void setDirection(const vec3 &dir);
    void setFocus(const vec3 &pos);
    void setAmbientIntensity(float ai);

    void calculateCamera();
    bool cullLight(Camera *cam);

};

