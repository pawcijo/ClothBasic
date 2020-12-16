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

  //CPU Data
  std::vector<Particle> particles;
  std::vector<Constraint> constraints;
  unsigned particlesWidthNumber;
  unsigned particlesHeightNumber;


  //GPU DATA
  std::vector<glm::vec4> positionData; //init with particle number
  std::vector<glm::vec4> oldPositionData;
  std::vector<glm::vec4> accelerationsData;
  std::vector<glm::vec4> constraintsData;
  std::vector<glm::vec2> constraintsTwoData;

  unsigned positionVbo;
  unsigned oldPositionSSbo;
  unsigned accelerationSSbo;
  unsigned constraintSSbo;
  unsigned constraintTwoSSbo;

  //GPU DRAW STUFF
      //Vertex Array buffer
    unsigned VAO; //simply verticies (id)  this is
    // Vertex Buffer object
    //unsigned VBO; //simpy object of verticies (id)  this is  positionSSbo
    // Element buffer object
    unsigned EBO; // simply object of indicies (id)

  
  unsigned indiciesSize;
  std::vector<unsigned> indicies;

  void generateBuffers(unsigned particleNumber,unsigned constraintNumber);

  
  void MakeConstriant(Particle *particleA, Particle *particleB);

  unsigned int instancedArray;
  unsigned int ParticleVAO;
  std::vector<glm::mat4*> modelMatrices;

public:
  Cloth(float width, float height, unsigned partilceNumberWidth,
        unsigned particleNumberHeight);

  ~Cloth();

  Cloth() = default;

  std::vector<Particle>& GetParticles();
  Particle *GetParticle(int x, int y);

  // First X second Y
  std::pair<unsigned,unsigned> GetClothSize() const;

  void AddForceToParticle(glm::vec3 force,unsigned width,unsigned height);

  void SetForceToParticle(glm::vec3 force,unsigned width,unsigned height);


  unsigned getContraintSize() const; 

  unsigned getParticlesNumber() const;


  //NEW implementation for GPU

  void AddForceToParticle_2(glm::vec3 force,unsigned width,unsigned height);

  void SetForceToParticle_2(glm::vec3 force,unsigned width,unsigned height);


  std::vector<glm::vec4>& getPositionData();
  std::vector<glm::vec4>& getOldPositionData();
  std::vector<glm::vec4>& getAcelerationsData();
  std::vector<glm::vec4>& getConstraintsData();
  std::vector<glm::vec2>& getConstraintsTwoData();


  void Draw(Shader *shader,Transform &transform);

  /*
    Function Update  - old timeStep
  */
  void Update(float elapsedTime,int CONSTRAINT_ITERATIONS);

  void AddForce(glm::vec3 force); // add gravity or any other force to all particles
};
