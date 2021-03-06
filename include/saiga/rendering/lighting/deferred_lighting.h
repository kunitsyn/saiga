/**
 * Copyright (c) 2017 Darius Rückert
 * Licensed under the MIT License.
 * See LICENSE file for more information.
 */

#pragma once

#include "saiga/opengl/vertex.h"
#include "saiga/camera/camera.h"
#include "saiga/opengl/framebuffer.h"
#include "saiga/opengl/indexedVertexBuffer.h"
#include "saiga/opengl/shader/basic_shaders.h"
#include "saiga/opengl/query/gpuTimer.h"

namespace Saiga {

class PointLightShader;
class SpotLightShader;
class DirectionalLightShader;
class BoxLightShader;
class LightAccumulationShader;

class SpotLight;
class PointLight;
class DirectionalLight;
class BoxLight;

class Program;

struct DeferredLightingShaderNames{
    std::string pointLightShader = "lighting/light_point.glsl";
    std::string spotLightShader = "lighting/light_spot.glsl";
    std::string directionalLightShader = "lighting/light_directional.glsl";
    std::string boxLightShader = "lighting/light_box.glsl";
    std::string debugShader = "lighting/debugmesh.glsl";
    std::string stencilShader = "lighting/stenciltest.glsl";
    std::string lightAccumulationShader = "lighting/lightaccumulation.glsl";
};


class SAIGA_GLOBAL DeferredLighting{
    friend class LightingController;
private:
    int width,height;
    std::shared_ptr<MVPColorShader>  debugShader;
    std::shared_ptr<MVPTextureShader>  textureShader;
    UniformBuffer shadowCameraBuffer;

    //the vertex position is sufficient. no normals and texture coordinates needed.
    typedef IndexedVertexBuffer<Vertex,GLushort> lightMesh_t;



    std::shared_ptr<PointLightShader>  pointLightShader, pointLightShadowShader, pointLightVolumetricShader;
    lightMesh_t pointLightMesh;
    std::vector< std::shared_ptr<PointLight> > pointLights;

    std::shared_ptr<SpotLightShader>  spotLightShader, spotLightShadowShader, spotLightVolumetricShader;
    lightMesh_t spotLightMesh;
    std::vector< std::shared_ptr<SpotLight> > spotLights;

    std::shared_ptr<BoxLightShader>  boxLightShader, boxLightShadowShader, boxLightVolumetricShader;
    lightMesh_t boxLightMesh;
    std::vector< std::shared_ptr<BoxLight> > boxLights;

    std::shared_ptr<DirectionalLightShader>  directionalLightShader,directionalLightShadowShader;
    lightMesh_t directionalLightMesh;
    std::vector< std::shared_ptr<DirectionalLight> > directionalLights;



    //    std::shared_ptr<MVPTextureShader>  blitDepthShader;
    std::shared_ptr<LightAccumulationShader>  lightAccumulationShader;

    std::shared_ptr<MVPShader> stencilShader;
    GBuffer &gbuffer;


    bool lightDepthTest = true;
    bool stencilCulling = true;
    bool drawDebug = true;

    bool useTimers = true;

    bool backFaceShadows = false;
    float shadowOffsetFactor = 4;
    float shadowOffsetUnits = 10;
    bool renderVolumetric = false;

    std::vector<FilteredMultiFrameOpenGLTimer> timers2;
    std::vector<std::string> timerStrings;
    void startTimer(int timer){if(useTimers)timers2[timer].startTimer();}
    void stopTimer(int timer){if(useTimers)timers2[timer].stopTimer();}
    float getTime(int timer) { if (!useTimers) return 0; return timers2[timer].getTimeMS(); }
public:
    vec4 clearColor = vec4(0);
    int totalLights;
    int visibleLights;
    int renderedDepthmaps;
    int currentStencilId = 0;

    int shadowSamples = 16; //Quadratic number (1,4,9,16,...)

    std::shared_ptr<Texture> ssaoTexture;

    std::shared_ptr<Texture> lightAccumulationTexture;
    std::shared_ptr<Texture> volumetricLightTexture;
    Framebuffer lightAccumulationBuffer;

    DeferredLighting(GBuffer &gbuffer);
    DeferredLighting& operator=(DeferredLighting& l) = delete;
    ~DeferredLighting();

    void init(int width, int height, bool _useTimers);
    void resize(int width, int height);

    void loadShaders(const DeferredLightingShaderNames& names = DeferredLightingShaderNames());

    void setRenderDebug(bool b){drawDebug = b;}
    void createLightMeshes();

    std::shared_ptr<DirectionalLight> createDirectionalLight();
    std::shared_ptr<PointLight> createPointLight();
    std::shared_ptr<SpotLight> createSpotLight();
    std::shared_ptr<BoxLight> createBoxLight();

    void removeLight(std::shared_ptr<DirectionalLight> l);
    void removeLight(std::shared_ptr<PointLight> l);
    void removeLight(std::shared_ptr<SpotLight> l);
    void removeLight(std::shared_ptr<BoxLight> l);


    void initRender();
    void render(Camera *cam);
    void renderLightAccumulation();
    void renderDepthMaps(Program *renderer );
    void renderDebug(Camera *cam);


    void setShader(std::shared_ptr<SpotLightShader>  spotLightShader, std::shared_ptr<SpotLightShader>  spotLightShadowShader);
    void setShader(std::shared_ptr<PointLightShader>  pointLightShader,std::shared_ptr<PointLightShader>  pointLightShadowShader);
    void setShader(std::shared_ptr<DirectionalLightShader>  directionalLightShader,std::shared_ptr<DirectionalLightShader>  directionalLightShadowShader);
    void setShader(std::shared_ptr<BoxLightShader>  boxLightShader,std::shared_ptr<BoxLightShader>  boxLightShadowShader);

    void setDebugShader(std::shared_ptr<MVPColorShader>  shader);
    void setStencilShader(std::shared_ptr<MVPShader> stencilShader);

    //add the volumetric light texture that was previously rendered to the scene
    void applyVolumetricLightBuffer();


    void cullLights(Camera *cam);

    void printTimings();
    void renderImGui(bool* p_open = NULL);

private:

    void blitGbufferDepthToAccumulationBuffer();
    void setupStencilPass();
    void setupLightPass(bool isVolumetric);

    template<typename T,typename shader_t>
    void renderLightVolume(lightMesh_t &mesh, T obj, Camera *cam, shader_t shader , shader_t shaderShadow, shader_t shaderVolumetric);


    void renderDirectionalLights(Camera *cam, bool shadow);

};


template<typename T,typename shader_t>
inline void DeferredLighting::renderLightVolume(lightMesh_t &mesh, T obj, Camera *cam, shader_t shaderNormal , shader_t shaderShadow, shader_t shaderVolumetric){
    if(!obj->shouldRender())
        return;

    if(stencilCulling){
        setupStencilPass();
        stencilShader->bind();

        obj->bindUniformsStencil(*stencilShader);
        mesh.bindAndDraw();
        stencilShader->unbind();
    }

    setupLightPass(obj->isVolumetric());
    shader_t shader = (obj->hasShadows() ?  (obj->isVolumetric() ? shaderVolumetric : shaderShadow) : shaderNormal);
    shader->bind();
    shader->DeferredShader::uploadFramebuffer(&gbuffer);
    shader->uploadScreenSize(vec2(width,height));

    obj->bindUniforms(shader,cam);
    mesh.bindAndDraw();
    shader->unbind();


}

}
