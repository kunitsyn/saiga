#pragma once

#include <saiga/opengl/indexedVertexBuffer.h>
#include <saiga/opengl/shader/basic_shaders.h>
#include <saiga/geometry/triangle_mesh.h>
#include <saiga/geometry/aabb.h>
#include <saiga/animation/boneVertex.h>
#include <saiga/animation/animation.h>

#include <saiga/camera/camera.h>


class SAIGA_GLOBAL Asset{
public:
    virtual void render(Camera *cam, const mat4 &model) = 0;
    virtual void renderDepth(Camera *cam, const mat4 &model) = 0;
    virtual void renderWireframe(Camera *cam, const mat4 &model) = 0;
    virtual void renderRaw() = 0;
};


template<typename vertex_t, typename index_t>
class SAIGA_GLOBAL BasicAsset : public Asset{
public:
    std::string name;
    aabb boundingBox;
    vec3 offset = vec3(0);

    MVPShader* shader  = nullptr;
    MVPShader* depthshader  = nullptr;
    MVPShader* wireframeshader  = nullptr;

    TriangleMesh<vertex_t,index_t> mesh;
    IndexedVertexBuffer<vertex_t,index_t> buffer;

    /**
     * Use these for simple inefficient rendering.
     * Every call binds and unbinds the shader and uploads the camera matrices again.
     */

    virtual void render(Camera *cam, const mat4 &model) override;
    virtual void renderDepth(Camera *cam, const mat4 &model) override;
    virtual void renderWireframe(Camera *cam, const mat4 &model) override;

    /**
     * Renders the mesh.
     * This maps to a single glDraw call and nothing else, so the shader
     * has to be setup before this renderRaw is called.
     */
    virtual void renderRaw() override;


    void create(std::string name, MVPShader* shader, MVPShader* depthshader, MVPShader* wireframeshader, bool normalizePosition=false, bool ZUPtoYUP=false);


    void normalizePosition();

    void normalizeScale();
    /**
     * Transforms the vertices and normals that the up axis is Y when before the up axis was Z.
     *
     * Many 3D CAD softwares (i.e. Blender) are using a right handed coordinate system with Z pointing upwards.
     * This frameworks uses a right haned system with Y pointing upwards.
     */


    void ZUPtoYUP();

};

template<typename vertex_t, typename index_t>
void BasicAsset<vertex_t,index_t>::render(Camera *cam, const mat4 &model)
{
    shader->bind();
    shader->uploadAll(model,cam->view,cam->proj);

//    glEnable(GL_POLYGON_OFFSET_FILL);
//    glPolygonOffset(1.0f,1.0f);

    buffer.bindAndDraw();

//    glDisable(GL_POLYGON_OFFSET_FILL);

    shader->unbind();
}

template<typename vertex_t, typename index_t>
void BasicAsset<vertex_t,index_t>::renderDepth(Camera *cam, const mat4 &model)
{
    depthshader->bind();
    depthshader->uploadAll(model,cam->view,cam->proj);
    buffer.bindAndDraw();
    depthshader->unbind();
}

template<typename vertex_t, typename index_t>
void BasicAsset<vertex_t,index_t>::renderWireframe(Camera *cam, const mat4 &model)
{
    wireframeshader->bind();
    wireframeshader->uploadAll(model,cam->view,cam->proj);

//    glEnable(GL_POLYGON_OFFSET_LINE);

    //negative values shifts the wireframe towards the camera,
    //but a non zero factors does strange things for lines and increases
    //the depth on lines with high slope towards the camera by too much.
    //a visually better solution is to shift the triangles back a bit glPolygonOffset(1,1);
    //and draw the wireframe without polygon offset.
//    glPolygonOffset(0.0f,-500.0f);

    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    buffer.bindAndDraw();
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
//    glDisable(GL_POLYGON_OFFSET_LINE);

    wireframeshader->unbind();
}

template<typename vertex_t, typename index_t>
void BasicAsset<vertex_t,index_t>::renderRaw()
{
    buffer.bindAndDraw();
}

template<typename vertex_t, typename index_t>
void BasicAsset<vertex_t,index_t>::normalizePosition()
{
    offset = boundingBox.getPosition();
    mat4 t = glm::translate(mat4(),-offset);
    mesh.transform(t);
    boundingBox.setPosition(vec3(0));
}


template<typename vertex_t, typename index_t>
void BasicAsset<vertex_t,index_t>::normalizeScale()
{
    //TODO
    vec3 d = boundingBox.max - boundingBox.min;
    int m = -1;
    int mi = -1;

    for(int i = 0 ; i < 3 ; ++i){

    }


    mat4 t = glm::translate(mat4(),-offset);
    mesh.transform(t);
    boundingBox.setPosition(vec3(0));
}



template<typename vertex_t, typename index_t>
void BasicAsset<vertex_t,index_t>::ZUPtoYUP()
{
    const mat4 m(
                1, 0, 0, 0,
                0, 0, -1, 0,
                0, 1, 0, 0,
                0, 0, 0, 1
                );
    mesh.transform(m);
    mesh.transformNormal(m);
}

template<typename vertex_t, typename index_t>
void BasicAsset<vertex_t,index_t>::create(std::string name, MVPShader* shader, MVPShader* depthshader, MVPShader* wireframeshader, bool normalizePosition, bool ZUPtoYUP){

    this->name = name;
    this->shader = shader;
    this->depthshader = depthshader;
    this->wireframeshader = wireframeshader;
    this->boundingBox = mesh.calculateAabb();

    if(ZUPtoYUP){
        this->ZUPtoYUP();
    }

    if(normalizePosition){
        this->normalizePosition();
    }


    mesh.createBuffers(buffer);



}