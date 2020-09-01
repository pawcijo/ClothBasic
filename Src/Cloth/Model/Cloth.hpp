#pragma once

#include <memory>
#include <vector>

#include "Shader/Shader.hpp"
#include <glm/glm.hpp>

#include "Cloth/Constraint/Constraint.hpp"
#include "Cloth/Particle/Particle.hpp"
class Cloth {

  unsigned particlesWidthNumber;
  unsigned particlesHeightNumber;

  std::vector<Particle> particles;
  std::vector<Constraint> constraints;

  
  void MakeConstriant(Particle *particleA, Particle *particleB);

  unsigned int instancedArray;
  unsigned int ParticleVAO;
  std::vector<glm::mat4*> modelMatrices;

public:
  Cloth(float width, float height, unsigned partilceNumberWidth,
        unsigned particleNumberHeight);

  std::vector<Particle>& GetParticles();
  Particle *GetParticle(int x, int y);

  // First X second Y
  std::pair<unsigned,unsigned> GetClothSize();

  void AddForceToParticle(glm::vec3 force,unsigned width,unsigned height);

  void SetForceToParticle(glm::vec3 force,unsigned width,unsigned height);

  /*
    Function Update  - old timeStep
  */
  void Update(float elapsedTime,int CONSTRAINT_ITERATIONS);

  void AddForce(glm::vec3 force); // add gravity or any other force to all particles
};
