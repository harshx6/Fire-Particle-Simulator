#ifndef PARTICLE_GENERATOR_H
#define PARTICLE_GENERATOR_H
#include <vector>

#include <GLEW/GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "src/renderer/opengleShader.h"
#include "src/renderer/opengltexture.h"


// Represents a single particle and its state
struct Particle {
    glm::vec3 vPosition;
    glm::vec3 vVelocity;
    glm::vec4 vColor;
    float fLifeTime;

    Particle() : vPosition(0.0f), vVelocity(0.0f), vColor(1.0f), fLifeTime(0.0f) { }
};


// ParticleGenerator acts as a container for rendering a large number of 
// particles by repeatedly spawning and updating particles and killing 
// them after a given amount of time.
class ParticleGenerator
{
public:
    // constructor
    ParticleGenerator(openglShader shader, opengl2Dtexture tex,  unsigned int amount, float min_time, float max_time, glm::vec3 color);
    // update all particles
    void Update(float dt, unsigned int newParticles, glm::vec3 offset = glm::vec3(0.0f));
    // render all particles
    void Draw(glm::vec3, glm::vec3);

    void spawnparticles(int amount, glm::vec3 min_Particle_vel, glm::vec3 max_Particle_vel);


    glm::vec3 CenterPos;
    glm::vec3 GeneratorConstSpeed = glm::vec3(5.0f, 5.0f, 0.0f);

    openglShader shader;
    opengl2Dtexture m_tex;
private:
    // state
    std::vector<Particle> particles;
    unsigned int amount;
    unsigned int VAO;    unsigned int IBO;


    float min_time, max_time;
    glm::vec3 color;


    // initializes buffer and vertex attributes
    void init();
      
    void UpdateParticle(int index, float Dt);

};

#endif