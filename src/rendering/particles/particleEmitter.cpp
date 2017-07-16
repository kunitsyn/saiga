#include "saiga/rendering/particles/particleEmitter.h"
#include "saiga/rendering/particles/particlesystem.h"

namespace Saiga {

ParticleEmitter::ParticleEmitter(){

}

void ParticleEmitter::setParticlesPerTick(float c)
{
    particlesPerTick = c;
}

void ParticleEmitter::setParticlesPerSecond(float c)
{
    particlesPerTick = c * ParticleSystem::secondsPerTick;
}


void ParticleEmitter::apply(ParticleSystem& ps)
{
    time += particlesPerTick;

    int c = 0;
    while(time>=1.0f){
        time -= 1.0f;
        c++;
    }

    if(c>0){
        spawnParticles(c,ps);
        ps.flush();
    }
}




SphericalParticleEmitter::SphericalParticleEmitter()
{

}

void SphericalParticleEmitter::spawnParticles(int count,ParticleSystem& ps)
{

    for(int i=0;i<count;++i){
        Particle p;
        p.position = vec4(this->getPosition(),1);
        p.velocity = vec4(glm::sphericalRand(1.0f),velocity);
        p.lifetime = lifetime;
        p.fadetime = 0;
        p.image = rand()%4;

//        p.velocity = vec4(0,1,0,1);
        p.orientation = Particle::BILLBOARD;
        ps.addParticle(p);
    }

}


ConaParticleEmitter::ConaParticleEmitter()
{

}

void ConaParticleEmitter::spawnParticles(int count,ParticleSystem& ps)
{

    for(int i=0;i<count;++i){
        Particle p;
        p.position = vec4(this->getPosition(),1);
        p.velocity = vec4(sampleCone(coneDirection,glm::radians(coneAngle)),velocity);
        p.lifetime = lifetime;
        p.image = 4;
        p.force = vec4(0,-1,0,0);
        p.orientation = Particle::VELOCITY;
        ps.addParticle(p);
    }

}

}
