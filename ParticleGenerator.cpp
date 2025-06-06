#include "ParticleGenerator.h"
#include "glm/gtc/random.hpp"
#include <math.h>

ParticleGenerator::ParticleGenerator(openglShader shader, opengl2Dtexture tex, unsigned int amount, float min_time, float max_time, glm::vec3 color)
    :amount(amount), shader(shader), min_time(min_time), max_time(max_time), color(color), m_tex(tex)
{
    //m_tex = opengl2Dtexture("asset/Green_fabric.jpg");

    this->init();
}



void ParticleGenerator::Update(float dt, unsigned int new_particles, glm::vec3 offset)
{
    for (int i = 0; i < particles.size(); i++)
    {
        UpdateParticle(i, dt);
    }

}

// render all particles
void ParticleGenerator::Draw(glm::vec3 camera_position, glm::vec3 scale)
{
    // use additive blending to give it a 'glow' effect
    glEnable(GL_BLEND);
    //  glBlendEquation(GL_ADD);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glDepthFunc(GL_ALWAYS);

    for (int i = 0; i < particles.size(); i++)
    {
        if (particles[i].fLifeTime > 0.0f)
        {
            this->shader.Bind();
            this->m_tex.bindTextureToSlot(0);
            this->shader.setTextureSampler("sprite", m_tex.GetTextureSlot());
            this->shader.setUniformFloat("fTime", particles[i].fLifeTime);

            float angle;
            glm::vec3 axis;

            {
                glm::vec3 face_vector = glm::vec3(0, 0, 1.0f); //Face of the Quads this varies z = -1.0f or z =1.0f based on Handedness 
                glm::vec3 DirectionVector = glm::normalize(camera_position - particles[i].vPosition);
                axis = glm::cross(face_vector, DirectionVector);
                axis = glm::normalize(axis);

                float cosangle = glm::dot(face_vector, DirectionVector);
                angle = glm::acos(cosangle);
            }

            glm::mat4 rot = glm::rotate(glm::mat4(1.0f), angle, axis);

            this->shader.setUniformVec4("color", particles[i].vColor);
            this->shader.setUniformMat4("model", glm::translate(glm::mat4(1.0f), particles[i].vPosition) * rot * glm::scale(glm::mat4(1.0f), scale));



            //this->texture.Bind();
            glBindVertexArray(this->VAO);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            glBindVertexArray(0);
        }
        else
        {
            particles.erase(particles.begin() + i);
        }
    }

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // don't forget to reset to default blending mode
}

void ParticleGenerator::spawnparticles(int amount, glm::vec3 min_Particle_vel, glm::vec3 max_Particle_vel)
{
    for (int index = 0; index < amount; index++)
    {
        particles.push_back(Particle());

        particles.back().vPosition = CenterPos + glm::ballRand(1.0f);

        //set new velocity 
        particles.back().vVelocity = glm::linearRand(min_Particle_vel, max_Particle_vel);
        /*if (rand() % 2 == 0) particles.back().vVelocity.x *= -1;
        if (rand() % 2 == 0) particles.back().vVelocity.z *= -1;*/

        particles.back().fLifeTime = glm::linearRand(min_time, max_time);

        particles.back().vColor = glm::vec4(color, 1.0f);
    }
}

void ParticleGenerator::init()
{
    // set up mesh and attribute properties
    unsigned int VBO;
    float particle_quad[] = {
        0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f,

        0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 0.0f, 1.0f, 0.0f
    };
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(this->VAO);
    // fill mesh buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(particle_quad), particle_quad, GL_STATIC_DRAW);
    // set mesh attributes
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

    glBindVertexArray(0);



    // create this->amount default particle instances
    for (unsigned int i = 0; i < this->amount; ++i)
    {
        this->particles.push_back(Particle());
    }
}

void ParticleGenerator::UpdateParticle(int index, float Dt)
{
    {

        float ypos = particles[index].vPosition.y;
        float pi = atan(1) * 4;

        if (particles[index].vVelocity.y > 0) {
            //particles[index].vPosition += (particles[index].fLifeTime / max_time) * GeneratorConstSpeed * Dt * particles[index].vVelocity;
            particles[index].vPosition += (particles[index].fLifeTime / max_time) * glm::vec3(0.5 * pi / ypos, (2 * pi) / ypos, 0.5 * pi / ypos) * particles[index].vVelocity * Dt;
        }
        else

            particles[index].vPosition += 0.1f * (particles[index].fLifeTime / max_time) * glm::vec3(0.5 * pi / ypos, 0.5 * pi / ypos, 0.5 * pi / ypos) * particles[index].vVelocity * Dt;
        float color_factor = sqrt(5 - particles[index].fLifeTime);

        particles[index].vColor -= glm::vec4(color_factor * 0.006, color_factor * 0.008, color_factor * 0.01, 1.0f);
        if (particles[index].vPosition.y < 2)
            particles[index].vColor -= 0.05f;

        particles[index].fLifeTime -= Dt;
    }
    //particles[index].iType
}

