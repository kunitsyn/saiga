/**
 * Copyright (c) 2017 Darius Rückert 
 * Licensed under the MIT License.
 * See LICENSE file for more information.
 */

#include "advancedWindow.h"

#include "saiga/rendering/deferred_renderer.h"
#include "saiga/rendering/lighting/directional_light.h"
#include "saiga/opengl/shader/shaderLoader.h"

#include "saiga/geometry/triangle_mesh_generator.h"

AdvancedWindow::AdvancedWindow(OpenGLWindow *window): Program(window),
   tdo(window->getWidth(),window->getHeight())
{
    //this simplifies shader debugging
//    ShaderLoader::instance()->addLineDirectives = true;


    //create a perspective camera
    float aspect = window->getAspectRatio();
    camera.setProj(60.0f,aspect,0.1f,50.0f);
    camera.setView(vec3(0,5,10),vec3(0,0,0),vec3(0,1,0));
    camera.enableInput();
    //How fast the camera moves
    camera.movementSpeed = 10;
    camera.movementSpeedFast = 20;

    //Set the camera from which view the scene is rendered
    window->setCamera(&camera);


    //add this object to the keylistener, so keyPressed and keyReleased will be called
    SDL_EventHandler::addKeyListener(this);

    ObjAssetLoader assetLoader;


    auto cubeAsset = assetLoader.loadTexturedAsset("objs/box.obj");

    cube1.asset = cubeAsset;
    cube2.asset = cubeAsset;
    cube1.translateGlobal(vec3(3,1,0));
    cube1.calculateModel();

    cube2.translateGlobal(vec3(3,1,5));
    cube2.calculateModel();

    auto sphereAsset = assetLoader.loadBasicAsset("objs/teapot.obj");
    sphere.asset = sphereAsset;
    sphere.translateGlobal(vec3(-2,1,0));
    sphere.rotateLocal(vec3(0,1,0),180);
    sphere.calculateModel();

    groundPlane.asset = assetLoader.loadDebugPlaneAsset(vec2(20,20),1.0f,Colors::lightgray,Colors::gray);

    //create one directional light
    //sun = window->getRenderer()->lighting.createDirectionalLight();
    //sun->setDirection(vec3(-1,-3,-2));
    //sun->setColorDiffuse(LightColorPresets::DirectSunlight);
    //sun->setIntensity(0.5);
    //sun->setAmbientIntensity(0.1f);
    //sun->createShadowMap(2048,2048);
    //sun->enableShadows();

    //create 10 point lights in a circle
    for(int i = 0 ; i < 10 ; ++i){
        auto pl = window->getRenderer()->lighting.createPointLight();
        pl->setAttenuation(AttenuationPresets::Quadratic);
        pl->setIntensity(3);
        pl->setRadius(10);
        pl->setPosition(vec3(10,3,0));
        pl->rotateAroundPoint(vec3(0,3,0),vec3(0,1,0),i * 360/10);
        pl->setColorDiffuse(glm::linearRand(vec3(0),vec3(1)));
        pl->calculateModel();
        pl->createShadowMap(256,256);
        lights.push_back(pl);
    }


    textAtlas.loadFont("fonts/SourceSansPro-Regular.ttf",40,2,4,true);

    tdo.init(&textAtlas);
    tdo.borderX = 0.01f;
    tdo.borderY = 0.01f;
    tdo.paddingY = 0.000f;
    tdo.textSize = 0.04f;

    tdo.textParameters.setColor(vec4(1),0.1f);
    tdo.textParameters.setGlow(vec4(0,0,0,1),1.0f);

    tdo.createItem("Fps: ");
    tdo.createItem("Ups: ");
    tdo.createItem("Render Time: ");
    tdo.createItem("Update Time: ");


    cout<<"Program Initialized!"<<endl;
}

AdvancedWindow::~AdvancedWindow()
{
    //We don't need to delete anything here, because objects obtained from saiga are wrapped in smart pointers.
}

void AdvancedWindow::update(float dt){
    //Update the camera position
    camera.update(dt);


    //sun->fitShadowToCamera(&camera);
//    sun->fitNearPlaneToScene(sceneBB);

    int  fps = (int) glm::round(1000.0/parentWindow->fpsTimer.getTimeMS());
    tdo.updateEntry(0,fps);

    int  ups = (int) glm::round(1000.0/parentWindow->upsTimer.getTimeMS());
    tdo.updateEntry(1,ups);

    float renderTime = parentWindow->getRenderer()->getTime(Deferred_Renderer::TOTAL);
    tdo.updateEntry(2,renderTime);

    float updateTime = parentWindow->updateTimer.getTimeMS();
    tdo.updateEntry(3,updateTime);


    for(auto l : lights){
        l->rotateAroundPoint(vec3(0,3,0),vec3(0,1,0),rotationSpeed);
        l->calculateModel();
    }
    //    sphere.rotateLocal(vec3(0,1,0),rotationSpeed);
    //    sphere.calculateModel();
}

void AdvancedWindow::interpolate(float dt, float interpolation) {
    //Update the camera rotation. This could also be done in 'update' but
    //doing it in the interpolate step will reduce latency
    camera.interpolate(dt,interpolation);
}

void AdvancedWindow::render(Camera *cam)
{
    //Render all objects from the viewpoint of 'cam'
    groundPlane.render(cam);
    cube1.render(cam);
    cube2.render(cam);
    sphere.render(cam);
}

void AdvancedWindow::renderDepth(Camera *cam)
{
    //Render the depth of all objects from the viewpoint of 'cam'
    //This will be called automatically for shadow casting light sources to create shadow maps
    groundPlane.renderDepth(cam);
    cube1.renderDepth(cam);
    cube2.renderDepth(cam);
    sphere.render(cam);
}

void AdvancedWindow::renderOverlay(Camera *cam)
{
    //The skybox is rendered after lighting and before post processing
//    skybox.render(cam);
}

void AdvancedWindow::renderFinal(Camera *cam)
{

    //The final render path (after post processing).
    //Usually the GUI is rendered here.

    tdo.layout.cam.calculateModel();
    tdo.layout.cam.recalculateMatrices();
    tdo.layout.cam.recalculatePlanes();

    parentWindow->getRenderer()->bindCamera(&tdo.layout.cam);
    tdo.render();


    {
        ImGui::SetNextWindowPos(ImVec2(50, 400), ImGuiSetCond_FirstUseEver);
        ImGui::SetNextWindowSize(ImVec2(400,200), ImGuiSetCond_FirstUseEver);
        ImGui::Begin("An Imgui Window :D");

        ImGui::SliderFloat("Rotation Speed",&rotationSpeed,0,10);


        if(ImGui::Checkbox("Point Light Shadows",&pointLightShadows)){
            for(auto l : lights){
                l->setCastShadows(pointLightShadows);
            }
        }

        ImGui::End();
    }


}


void AdvancedWindow::keyPressed(SDL_Keysym key)
{
    switch(key.scancode){
    case SDL_SCANCODE_ESCAPE:
        parentWindow->close();
        break;
    case SDL_SCANCODE_BACKSPACE:
        parentWindow->getRenderer()->printTimings();
        break;
    case SDL_SCANCODE_R:
        ShaderLoader::instance()->reload();
        break;
    case SDL_SCANCODE_F12:
        parentWindow->screenshot("screenshot.png");
        break;
    default:
        break;
    }
}

void AdvancedWindow::keyReleased(SDL_Keysym key)
{
}



