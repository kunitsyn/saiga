/**
 * Copyright (c) 2017 Darius Rückert 
 * Licensed under the MIT License.
 * See LICENSE file for more information.
 */

#include "saiga/image/imageGenerator.h"
#include "saiga/image/templatedImage.h"

namespace Saiga {

std::shared_ptr<Image> ImageGenerator::checkerBoard(vec3 color1, vec3 color2, int quadSize, int numQuadsX, int numQuadsY)
{
    Image* image = new Image();

    image->width = quadSize * numQuadsX;
    image->height = quadSize * numQuadsY;

    image->Format() = ImageFormat(3,8,ImageElementFormat::UnsignedNormalized,true);
//    image->srgb =true;
//    image->bitDepth = 8;
//    image->channels = 3;

    image->create();

    uint8_t r1=color1.x*255.0f,
            g1=color1.y*255.0f,
            b1=color1.z*255.0f;


    uint8_t r2=color2.x*255.0f,
            g2=color2.y*255.0f,
            b2=color2.z*255.0f;

    bool black = true;
    for(int qx = 0; qx<numQuadsX; ++qx){
        for(int qy = 0; qy<numQuadsY; ++qy){


            for(int i=0; i<quadSize;++i){
                for(int j=0; j<quadSize;++j){
                    if(black)
                        image->setPixel(qx*quadSize+i,qy*quadSize+j,r1,g1,b1);
                    else
                        image->setPixel(qx*quadSize+i,qy*quadSize+j,r2,g2,b2);


                }
            }

            black = !black;
        }
        black = !black;
    }



    return std::shared_ptr<Image>(image);

}

std::shared_ptr<Image> ImageGenerator::randomNormalized(int width, int height)
{
    typedef TemplatedImage<4,8,ImageElementFormat::UnsignedNormalized,true> image_t;
    image_t* image = new image_t(width,height);
    for(int i = 0 ; i < height ; ++i){
        for(int j = 0 ; j < width ; ++j){
            image_t::TexelType texel;
            texel.r = glm::linearRand(0,255);
            texel.g = glm::linearRand(0,255);
            texel.b = glm::linearRand(0,255);
            texel.a = glm::linearRand(0,255);
            image->setTexel(j,i,texel);
        }
    }
    return std::shared_ptr<Image>(image);
}

}
