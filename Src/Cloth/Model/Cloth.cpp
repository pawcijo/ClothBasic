
#include "Cloth/Model/Cloth.hpp"

#include <iostream>

Particle *Cloth::GetParticle(int x, int y) {
  return &particles[y * particlesWidthNumber + x];
}

void Cloth::MakeConstriant(Particle *p1, Particle *p2) {
  constraints.push_back(Constraint(p1, p2));
}

Cloth::Cloth(float width, float height, unsigned particleWidth,
             unsigned particleHeight)
    : particlesWidthNumber(particleWidth),
      particlesHeightNumber(particleWidth) {

  particles.resize(particleWidth *
                   particleWidth); // I am essentially using this vector
                                   // as an array with room for
                                   // num_particles_width*num_particles_height
                                   // particles

  // creating particles in a grid of particles from (0,0,0) to (width,-height,0)
  for (int x = 0; x < particlesWidthNumber; x++) {
    for (int y = 0; y < particlesHeightNumber; y++) {
      glm::vec3 pos =
          glm::vec3(width * (x / (float)particlesWidthNumber),
                    -height * (y / (float)particlesHeightNumber), 0);
      particles[y * particlesWidthNumber + x] =
          Particle(pos); // insert particle in column x at y'th row
    }
  }

  // Connecting immediate neighbor particles with constraints (distance 1 and
  // sqrt(2) in the grid)
  for (int x = 0; x < particlesWidthNumber; x++) {
    for (int y = 0; y < particlesHeightNumber; y++) {

      if (x < particlesWidthNumber - 1)
        MakeConstriant(GetParticle(x, y), GetParticle(x + 1, y));

      if (y < particlesHeightNumber - 1)
        MakeConstriant(GetParticle(x, y), GetParticle(x, y + 1));

      if (x < particlesWidthNumber - 1 && y < particlesHeightNumber - 1)
        MakeConstriant(GetParticle(x, y), GetParticle(x + 1, y + 1));

      if (x < particlesWidthNumber - 1 && y < particlesHeightNumber - 1)
        MakeConstriant(GetParticle(x + 1, y), GetParticle(x, y + 1));
    }
  }

  // Connecting secondary neighbors with constraints (distance 2 and sqrt(4) in
  // the grid)
  for (int x = 0; x < particlesWidthNumber; x++) {
    for (int y = 0; y < particlesHeightNumber; y++) {
      if (x < particlesWidthNumber - 2)
        MakeConstriant(GetParticle(x, y), GetParticle(x + 2, y));
      if (y < particlesHeightNumber - 2)
        MakeConstriant(GetParticle(x, y), GetParticle(x, y + 2));
      if (x < particlesWidthNumber - 2 && y < particlesHeightNumber - 2)
        MakeConstriant(GetParticle(x, y), GetParticle(x + 2, y + 2));
      if (x < particlesWidthNumber - 2 && y < particlesHeightNumber - 2)
        MakeConstriant(GetParticle(x + 2, y), GetParticle(x, y + 2));
    }
  }

  // making the upper left most two and right most two particles unmovable
  for (int i = 0; i < 2; i++) {
    GetParticle(0 + i, 0)->SetMoveable(false);
    GetParticle(particlesWidthNumber - 1 - i, 0)->SetMoveable(false);
  }

}

std::vector<Particle> &Cloth::GetParticles() { return particles; }

void Cloth::Update(float elapsedTime, int CONSTRAINT_ITERATIONS) {

  for (int i = 0; i < CONSTRAINT_ITERATIONS;
       i++) // iterate over all constraints several times
  {
    for (auto &constraint : constraints) {
      constraint.ResolveConstraint(); // satisfy constraint.
    }
  }

  for (auto &particle : particles) {
    particle.Update(elapsedTime); // calculate the position of each particle at
                                  // the next time step.
  }
}


 // X Y
std::pair<unsigned, unsigned> Cloth::GetClothSize() {
  return std::make_pair(particlesWidthNumber, particlesHeightNumber);
}

void Cloth::AddForceToParticle(glm::vec3 force, unsigned x, unsigned y) {

  Particle *particle = GetParticle(x, y);
  particle->AddForce(force);
  
}

void Cloth::SetForceToParticle(glm::vec3 force, unsigned x, unsigned y) {

  Particle *particle = GetParticle(x, y);
}

void Cloth::AddForce(glm::vec3 direction) {
  for (auto &particle : particles) {
    particle.AddForce(direction); // add the forces to each particle
  }
}
