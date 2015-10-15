#include "particle_generator.h"
#include "GLEW/glew.h"
#include <iostream>

// todo:
// - rate control
// - make constants not hardcoded

void ParticleGenerator::draw(float alpha_offset)
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  

    glPointSize(5.0f);
    glBegin(GL_POINTS);
    for (const Particle& p : particles)
    {
        glColor4f(1.0f, p.brightness*0.5f, 0.0f, p.brightness * 2.0f - alpha_offset);
        glVertex3f(p.x, p.y, p.z);
    }

    glEnd();
    glDisable(GL_BLEND);
}

void ParticleGenerator::advance(float dt)
{
    std::cout << "particles: " << particles.size() << std::endl;

    particles.push_back(make_particle());

    for (auto itr = particles.begin(); itr != particles.end(); /*in loop*/)
    {
        Particle& p = *itr;

        // position integration
        p.x += dt * p.vx;
        p.y += dt * p.vy;
        p.z += dt * p.vz;

        // add gravity
        p.vy += dt * (-9.8f);

        // add drag (air resistance)
        p.vx *= 0.99f;
        p.vy *= 0.99f;
        p.vz *= 0.99f;

        // reduce brightness
        p.brightness += dt * (-0.2f);

        // bounce off table, losing energy
        if (p.y < 0)
        {
            p.vy = -p.vy * 0.8f;
        }

        if (p.brightness < 0)
        {
            particles.erase(itr++);
        }
        else
        {
            itr++;
        }
    }
}

float ParticleGenerator::frand()
{
    return (rand() / float(RAND_MAX)) - 0.5f;
}

ParticleGenerator::Particle ParticleGenerator::make_particle()
{
    Particle p;
    p.x = 0.0f;
    p.y = 3.0f;
    p.z = 0.0f;
    p.vx = 0.0f + 4*frand();
    p.vy = 0.0f + 1*frand();
    p.vz = 0.0f + 4*frand();
    p.brightness = 1.0f + 0.2f*frand();

    return p;
}
