/**
 * Copyright (c) 2017 Darius Rückert 
 * Licensed under the MIT License.
 * See LICENSE file for more information.
 */

#include "saiga/animation/objLoader2.h"
#include "saiga/util/fileChecker.h"

#include <fstream>
#include <sstream>
#include <algorithm>

namespace Saiga {

std::vector<std::string> split(const std::string &s, char delim);


//======================================================================

ObjLoader2::ObjLoader2(const std::string &file):file(file)
{
    loadFile(file);
}

bool ObjLoader2::loadFile(const std::string &_file){
	this->file = _file;
    std::ifstream stream(file, std::ios::in);
    if(!stream.is_open()) {
        return false;
    }


    cout<<"objloader: loading file "<<file<<endl;

    ObjTriangleGroup tg;
    tg.startFace = 0;
    tg.faces = 0;
    triangleGroups.push_back(tg);

    while(!stream.eof()) {
        std::string line;
        std::getline(stream, line);
        //remove carriage return from windows
        line.erase( std::remove(line.begin(), line.end(), '\r'), line.end() );
        parseLine(line);
    }

    //finish last group
    ObjTriangleGroup &lastGroup = triangleGroups[triangleGroups.size()-1];
    lastGroup.faces = faces.size() - lastGroup.startFace;

    cout << ".obj parsing finished. " << "V " << vertices.size() << " N " << normals.size() << " T " << texCoords.size() << " F " << faces.size() << endl;



//    cout<<"number of vertices "<<vertices.size()<<" number of faces "<<faces.size()<<endl;
    createVertexIndexList();
//    cout<<"number of vertices "<<outVertices.size()<<" number of faces "<<outTriangles.size()<<endl;
    separateVerticesByGroup();
//    cout<<"number of vertices "<<outVertices.size()<<" number of faces "<<outTriangles.size()<<endl;

    calculateMissingNormals();

//    cout<<"objloader finished :)"<<endl;
    return true;
}

void ObjLoader2::separateVerticesByGroup()
{
    //make sure faces from different triangle groups do not reference the same vertex
    //needs to be called after createVertexIndexList()

    std::vector<int> vertexReference(outVertices.size(),INVALID_VERTEX_ID);

    for(int t = 0; t < (int)triangleGroups.size() ; ++t){
        ObjTriangleGroup &tg = triangleGroups[t];
        for(int i = 0 ; i < tg.faces ; ++i){
            ObjTriangle &face = outTriangles[i+tg.startFace];

            for(int j = 0 ; j < 3 ; ++j){
                int index = face.v[j];
                if(vertexReference[index] == INVALID_VERTEX_ID)
                    vertexReference[index] = t;
                if(vertexReference[index] != t){
                    //duplicate vertices
                    VertexNT v = outVertices[index];
                    int newIndex = outVertices.size();
                    outVertices.push_back(v);
                    face.v[j] = newIndex;
                    vertexReference.push_back(t);
//                    SAIGA_ASSERT(0);
                }
            }
        }
    }


}

void ObjLoader2::calculateMissingNormals()
{

}

void ObjLoader2::createVertexIndexList()
{
    std::vector<bool> vertices_used(vertices.size(),false);


    outVertices.resize(vertices.size());

    for(std::vector<IndexedVertex2> &f : faces){
        ObjTriangle fa;
        for(int i=0;i<3;i++){
            IndexedVertex2 &currentVertex = f[i];
            int vert = currentVertex.v;
            int norm = currentVertex.n;
            int tex = currentVertex.t;

            VertexNT verte;
            if(vert>=0){
                SAIGA_ASSERT(vert < (int)vertices.size());
                verte.position = vec4(vertices[vert],1);
            }
            if(norm>=0){
                SAIGA_ASSERT(norm < (int)normals.size());
                verte.normal = vec4(normals[norm],0);
            }
            if(tex>=0){
                SAIGA_ASSERT(tex < (int)texCoords.size());
                verte.texture = texCoords[tex];
            }


            int index = INVALID_VERTEX_ID;
            if(vertices_used[vert]){
                if(verte==outVertices[vert]){
                    index = vert;
                }
            }else{
                outVertices[vert] = verte;
                index = vert;
                vertices_used[vert] = true;
            }

            if(index==INVALID_VERTEX_ID){
                index = outVertices.size();
                outVertices.push_back(verte);
            }
            fa.v[i] = index;
        }

        outTriangles.push_back(fa);
    }
}

std::vector<std::vector<IndexedVertex2> > ObjLoader2::triangulateFace(const std::vector<IndexedVertex2> &f)
{
    std::vector<std::vector<IndexedVertex2>> newFaces;

    //more than 3 indices -> triangulate
    std::vector<IndexedVertex2> face;
    int cornerCount = 1;
    IndexedVertex2 startVertex, lastVertex;
    for(const IndexedVertex2 &currentVertex : f){
        if(cornerCount<=3){
            if(cornerCount==1)
                startVertex = currentVertex;
            face.push_back(currentVertex);
            if(cornerCount==3)
                newFaces.push_back(face);
        }else{
            face.resize(3);
            face[0] = lastVertex;
            face[1] = currentVertex;
            face[2] = startVertex;
            newFaces.push_back(face);
        }

        lastVertex = currentVertex;
        cornerCount++;
    }
    return newFaces;
}

void ObjLoader2::parseLine(const std::string &line)
{
    std::stringstream sstream(line);

    std::string header;
    sstream >> header;

    std::string rest;
    std::getline(sstream,rest);

    //remove first white space
    if(rest[0]==' ' && rest.size()>1){
        rest = rest.substr(1);
    }


    if(header == "#"){
    }else if(header == "usemtl"){
        parseUM(rest);
    }else if(header == "mtllib"){
        parseM(rest);
    }else if(header == "g"){
        //        cout<<"Found Group: "<<line<<endl;
    }else if(header == "o"){
        //        cout<<"Found Object: "<<line<<endl;
    }else if(header == "s"){
        //smooth shading
    }else if(header == "v"){
        parseV(rest);
    }else if(header == "vt"){
        parseVT(rest);
    }else if(header == "vn"){
        parseVN(rest);
    }else if(header == "f"){
        parseF(rest);
    }
}

void ObjLoader2::parseV(const std::string &line)
{
    std::stringstream sstream(line);
    vec3 v;
    sstream >> v;
    vertices.push_back(v);
}

void ObjLoader2::parseVT(const std::string &line)
{
    std::stringstream sstream(line);
    vec2 v;
    sstream >> v;
    texCoords.push_back(v);
}

void ObjLoader2::parseVN(const std::string &line)
{
    std::stringstream sstream(line);
    vec3 v;
    sstream >> v;
    normals.push_back(glm::normalize(v));
}

void ObjLoader2::parseF(std::string &line)
{

    //    std::replace( line.begin(), line.end(), '/', ' ');

    std::stringstream sstream(line);
    std::string t;

    //    cout<<"parse F "<<line<<endl;

    std::vector<IndexedVertex2> ivs;
    while(sstream >> t){
        ivs.push_back(parseIV(t));
    }

    auto nf = triangulateFace(ivs);

    //relative indexing, when the index is negativ
    for(auto &f : nf){
        for(auto &i : f){
            if(i.v < 0 && i.v != INVALID_VERTEX_ID){
                int absIndex = vertices.size() + i.v + 1;
                i.v = absIndex;
            }
            if(i.n < 0 && i.n != INVALID_VERTEX_ID){
                int absIndex = normals.size() + i.n + 1;
                i.n = absIndex;
            }
            if(i.t < 0 && i.t != INVALID_VERTEX_ID){
                int absIndex = texCoords.size() + i.t + 1;
                i.t = absIndex;
            }
        }
    }

    faces.insert(faces.end(),nf.begin(),nf.end());
}

//parsing index vertex
//examples:
//v1/vt1/vn1        12/51/1
//v1//vn1           51//4
IndexedVertex2 ObjLoader2::parseIV(std::string &line)
{
    IndexedVertex2 iv;
    std::vector<std::string> s = split(line, '/');
    if(s.size()>0 && s[0].size()>0)
        iv.v = std::atoi(s[0].c_str()) - 1;
    if(s.size()>1 && s[1].size()>0)
        iv.t = std::atoi(s[1].c_str()) - 1;
    if(s.size()>2 && s[2].size()>0)
        iv.n = std::atoi(s[2].c_str()) - 1;
    return iv;
}

void ObjLoader2::parseUM(const std::string &line)
{
    //finish current group and create new one
    if(!triangleGroups.empty()){
        ObjTriangleGroup &currentGroup = triangleGroups[triangleGroups.size()-1];
        currentGroup.faces = faces.size() - currentGroup.startFace;
    }
    ObjTriangleGroup newGroup;
    newGroup.startFace = faces.size();
    newGroup.material = materialLoader.getMaterial(line);
    triangleGroups.push_back(newGroup);


}

void ObjLoader2::parseM(const std::string &line)
{
    FileChecker fc;
    materialLoader.loadFile(fc.getRelative(file,line));
}

}
