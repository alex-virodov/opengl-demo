#ifndef __PARTICLEGENERATOR_H__
#define __PARTICLEGENERATOR_H__

#include <list>

class ParticleGenerator
{
private:
    struct Particle
    {
        float x, y, z;      // position
        float vx, vy, vz;   // velocity
        float brightness;
    };

    // Using list due to the need to delete particles from middle.
    // std::vector can still be more efficient because of less alloc/free
    // but even better to just have an 'active' flag and reuse particles.
    std::list<Particle> particles;

    Particle make_particle();

public:

    void draw(float alpha_offset = 0.0f);
    void advance(float dt);

    static float frand();
};


#endif