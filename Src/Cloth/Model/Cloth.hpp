#pragma once

#include <memory>
#include <vector>

#include "Shader/Shader.hpp"
#include <glm/glm.hpp>

#include "Cloth/Constraint/Constraint.hpp"
#include "Cloth/Particle/Particle.hpp"

struct ParticleStruct{

   float restDistance;

  //p1
  int m1Move,m2Move; 
  float p1Mass,m2Mass;
  float x1,y1,z1,m1;
  float x2,y2,z2,m2;

  //p2
  
};

/*
struct constraint{
  int p1Moveable;
  float p1X;
  float p1Y;
  float p1Z;
  float p1mass;
  
  float restDistance;

  int p2Moveable;
  float p2X;
  float p2Y;
  float P2Z;
  float p2mass;

}
*/




class Cloth {

  unsigned particlesWidthNumber;
  unsigned particlesHeightNumber;

  std::vector<Particle> particles;
  std::vector<Constraint> constraints;


  std::vector<glm::vec3> positionData; //init with particle number
  std::vector<glm::vec3> oldPositionData;
  std::vector<glm::vec3> accelerationsData;
  std::vector<glm::vec3> constraintsData;
  std::vector<glm::vec2> constraintsTwoData;

  unsigned positionSSbo;
  unsigned oldPositionSSbo;
  unsigned accelerationSSbo;
  unsigned constraintSSbo;
  unsigned constraintTwoSSbo;

  void generateBuffers(unsigned particleNumber);

  
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
