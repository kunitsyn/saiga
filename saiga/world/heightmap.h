#pragma once

#include "saiga/opengl/texture/texture.h"
#include "saiga/opengl/texture/cube_texture.h"
#include "saiga/opengl/indexedVertexBuffer.h"
#include "saiga/opengl/shader/basic_shaders.h"
#include "saiga/util/perlinnoise.h"
#include "saiga/geometry/triangle_mesh_generator.h"

namespace Saiga {

class SAIGA_GLOBAL Heightmap{
private:
    typedef TriangleMesh<Vertex,GLuint> mesh_t;
public:
    int n = 63;
     int m = (n+1)/4;

     int layers,w,h;

     float* heights;

     float heightScale = 20.0f;
     float minH = 125725;
     float maxH = -0125725;

     vec2 mapOffset = vec2(0);//vec2(50,50);
     vec2 mapScale = vec2(10);
     vec2 mapScaleInv = 1.0f/mapScale;

    std::vector<Image> heightmap;
    std::vector<Image> normalmap;

    std::vector<std::shared_ptr<Texture>> texheightmap;
    std::vector<std::shared_ptr<Texture>> texnormalmap;


    Heightmap(int layers, int w, int h);
    void setScale(vec2 mapScale, vec2 mapOffset = vec2(0));

    void createTextures();
    void createHeightmaps();
    void createHeightmapsFrom(const std::string& image);

    bool loadMaps();

private:

    void createInitialHeightmap();
    void createRemainingLayers();
    void createNormalmap();

    void normalizeHeightMap();

    void saveHeightmaps();
    void saveNormalmaps();

    float getHeight(int x, int y);
    float getHeight(int layer, int x, int y);
    float getHeightScaled(int x, int y);
    float getHeightScaled(int layer, int x, int y);
    void setHeight(int x, int y, float v);


};

}
