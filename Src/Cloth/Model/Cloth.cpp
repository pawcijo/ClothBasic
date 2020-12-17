
#include "Cloth/Model/Cloth.hpp"

#include <iostream>
#include <algorithm>
#include "Src/Util/ClothGeneration/ClothGeneration.hpp"

#include "Src/Util/DrawMode/DrawMode.hpp"

Particle *Cloth::GetParticle(int x, int y)
{
  return &particles[y * particlesWidthNumber + x];
}

void Cloth::MakeConstriant(Particle *p1, Particle *p2)
{
  constraints.push_back(Constraint(p1, p2));
}

void Cloth::generateBuffers(unsigned particleNumber, unsigned constraintNumber)
{

  //glGenVertexArrays(1, &VAO);
  //glBindVertexArray(VAO);

  indicies = genereteIndicies(std::pair<unsigned, unsigned>(particlesWidthNumber, particlesHeightNumber));
  indiciesSize = indicies.size() * sizeof(unsigned);

  //glGenBuffers(1, &positionVbo);
 // glGenBuffers(1, &oldPositionSSbo);
 // glGenBuffers(1, &accelerationSSbo);
 // glGenBuffers(1, &constraintSSbo);
 // glGenBuffers(1, &constraintTwoSSbo);

  //GL_STREAM_READ_ARB, GL_STATIC_READ_ARB, or GL_DYNAMIC_READ_ARB
  /*
  glBindBuffer(GL_ARRAY_BUFFER, positionVbo);
  glBufferData(GL_ARRAY_BUFFER, positionData.size() * sizeof(glm::vec4), NULL, GL_DYNAMIC_DRAW);
  glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

  glBindBuffer(GL_SHADER_STORAGE_BUFFER, oldPositionSSbo);
  glBufferData(GL_SHADER_STORAGE_BUFFER, oldPositionData.size() * sizeof(glm::vec4), NULL, GL_STREAM_COPY);
  glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

  glBindBuffer(GL_SHADER_STORAGE_BUFFER, accelerationSSbo);
  glBufferData(GL_SHADER_STORAGE_BUFFER, accelerationsData.size() * sizeof(glm::vec4), NULL, GL_STREAM_COPY);
  glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

  glBindBuffer(GL_SHADER_STORAGE_BUFFER, constraintSSbo);
  glBufferData(GL_SHADER_STORAGE_BUFFER, constraintsData.size() * sizeof(glm::vec4), NULL, GL_STREAM_COPY);
  gglUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

  glBindBuffer(GL_SHADER_STORAGE_BUFFER, constraintTwoSSbo);
  glBufferData(GL_SHADER_STORAGE_BUFFER, constraintsTwoData.size() * sizeof(glm::vec4), NULL, GL_STREAM_COPY);
  glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

  ssbos = new unsigned[5];
  ssbos[0] = positionVbo;
  ssbos[1] = oldPositionSSbo;
  ssbos[2] = accelerationSSbo;
  ssbos[3] = constraintSSbo;
  ssbos[4] = constraintTwoSSbo;

  glBindBuffer(GL_ARRAY_BUFFER, positionVbo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicies.size() * sizeof(unsigned), &indicies[0], GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
  */

  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);
  glGenBuffers(1, &EBO);

  glGenBuffers(1, &positionVbo);

  glBindVertexArray(VAO); 
  glBindBuffer(GL_ARRAY_BUFFER, positionVbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4)*positionData.size(), &positionData[0], GL_STATIC_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, positionVbo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicies.size() * sizeof(unsigned), &indicies[0], GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4*sizeof(GLfloat), (char*)0 + 0*sizeof(GLfloat));
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, positionVbo);

}

void Cloth::retriveData()
{
  int DataSize = positionData.size();

  // ------------------------------------------- POSITION ---------------------------
  glBindBuffer(GL_ARRAY_BUFFER, positionVbo);

  glm::vec4 *kekw = (glm::vec4 *)glMapBuffer(GL_ARRAY_BUFFER, GL_READ_ONLY);

  for (int i = 0; i < DataSize; i++)
  {
    positionData[i] = glm::vec4(kekw[i]);
  }

  glUnmapBuffer(GL_ARRAY_BUFFER);
  /*
  // ------------------------------------------- OLD POSITION ---------------------------

  glBindBuffer(GL_SHADER_STORAGE_BUFFER, oldPositionSSbo);

  glm::vec4 *kekw1 = (glm::vec4 *)glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY);

  for (int i = 0; i < DataSize; i++)
  {
    oldPositionData[i] = glm::vec4(kekw1[i]);
  }

  glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

  // ------------------------------------------- ACCELERATION ---------------------------

  glBindBuffer(GL_SHADER_STORAGE_BUFFER, accelerationSSbo);

  glm::vec4 *kekw2 = (glm::vec4 *)glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY);

  for (int i = 0; i < DataSize; i++)
  {
    accelerationsData[i] = glm::vec4(kekw2[i]);
  }

  glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

  // ------------------------------------------- CONTRAINT ---------------------------

  glBindBuffer(GL_SHADER_STORAGE_BUFFER, constraintSSbo);

  glm::vec4 *kekw3 = (glm::vec4 *)glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY);

  for (int i = 0; i < DataSize; i++)
  {
    constraintsData[i] = glm::vec4(kekw3[i]);
  }

  glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

  // ------------------------------------------- CONTRAINT2 ---------------------------

  glBindBuffer(GL_SHADER_STORAGE_BUFFER, constraintTwoSSbo);

  glm::vec4 *kekw4 = (glm::vec4 *)glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY);

  for (int i = 0; i < DataSize; i++)
  {
    constraintsTwoData[i] = glm::vec4(kekw4[i]);
  }


  
  glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
  */
}

Cloth::Cloth(float width, float height, unsigned particleWidth,
             unsigned particleHeight)
    : particlesWidthNumber(particleWidth),
      particlesHeightNumber(particleWidth)
{

  particles.resize(particleWidth *
                   particleWidth); // I am essentially using this vector
                                   // as an array with room for
                                   // num_particles_width*num_particles_height
                                   // particles

  // creating particles in a grid of particles from (0,0,0) to (width,-height,0)
  int index = 0;
  for (int x = 0; x < particlesWidthNumber; x++)
  {
    for (int y = 0; y < particlesHeightNumber; y++)
    {
      glm::vec3 pos =
          glm::vec3(width * (x / (float)particlesWidthNumber),
                    -height * (y / (float)particlesHeightNumber), 0);

      particles[y * particlesWidthNumber + x] =
          Particle(pos, index, x, y, 1); // insert particle in column x at y'th row
      index++;
    }
  }

  // Connecting immediate neighbor particles with constraints (distance 1 and
  // sqrt(2) in the grid)
  for (int x = 0; x < particlesWidthNumber; x++)
  {
    for (int y = 0; y < particlesHeightNumber; y++)
    {

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
  for (int x = 0; x < particlesWidthNumber; x++)
  {
    for (int y = 0; y < particlesHeightNumber; y++)
    {
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
  for (int i = 0; i < 2; i++)
  {
    GetParticle(0 + i, 0)->SetMoveable(false);
    GetParticle(particlesWidthNumber - 1 - i, 0)->SetMoveable(false);
  }

  //fill data

  for (int i = 0; i < particles.size(); i++)
  {
    positionData.push_back(glm::vec4(particles[i].GetPositionCopy(),
                                     particles[i].isMoveable()));
  }

  for (auto particle : particles)
  {
    oldPositionData.push_back(glm::vec4(particle.GetOldPositionCopy(), 0));
  }

  for (auto particle : particles)
  {
    accelerationsData.push_back(glm::vec4(particle.GetAccelerationCopy(), 0));
  }

  for (auto constraint : constraints)
  {
    std::vector<Particle>::iterator firstParticle = std::find(particles.begin(), particles.end(), *constraint.p1);
    std::vector<Particle>::iterator secondParticle = std::find(particles.begin(), particles.end(), *constraint.p2);
    int firstParticleIndex = std::distance(particles.begin(), firstParticle);
    int secondParticleIndex = std::distance(particles.begin(), secondParticle);
    constraintsData.push_back(glm::vec4(firstParticleIndex, secondParticleIndex, constraint.getRestDistance(), 0));
  }

  for (auto constraint : constraints)
  {
    constraintsTwoData.push_back(glm::vec4(constraint.p1->isMoveable(), constraint.p2->isMoveable(), 0, 0));
  }

  std::cout << "\n";
  std::cout << "Cloth buffers generation test:"
            << "\n";
  std::cout << "PositionData Size :" << positionData.size() << "\n";
  std::cout << "OldPositionData Size :" << oldPositionData.size() << "\n";
  std::cout << "AccelerationData Size :" << accelerationsData.size() << "\n";
  std::cout << "ConstraintsData Size :" << constraintsData.size() << "\n";
  std::cout << "ConstraintsTwoData Size :" << constraintsTwoData.size() << "\n";
  std::cout << "\n";

  generateBuffers(particlesWidthNumber * particlesHeightNumber, constraints.size());
}

std::vector<Particle> &Cloth::GetParticles() { return particles; }

void Cloth::Update(float elapsedTime, int CONSTRAINT_ITERATIONS)
{

  for (int i = 0; i < CONSTRAINT_ITERATIONS; i++) // iterate over all constraints several times
  {
    for (auto &constraint : constraints)
    {
      constraint.ResolveConstraint(); // satisfy constraint.
    }
  }

  for (auto &particle : particles)
  {
    particle.Update(elapsedTime); // calculate the position of each particle at
                                  // the next time step.
  }
}

// X Y
std::pair<unsigned, unsigned> Cloth::GetClothSize() const
{
  return std::make_pair(particlesWidthNumber, particlesHeightNumber);
}

unsigned Cloth::getParticlesNumber() const
{
  return static_cast<unsigned>(particles.size());
}

void Cloth::AddForceToParticle(glm::vec3 force, unsigned x, unsigned y)
{

  Particle *particle = GetParticle(x, y);
  particle->AddForce(force);
}

void Cloth::SetForceToParticle(glm::vec3 force, unsigned x, unsigned y)
{

  Particle *particle = GetParticle(x, y);
}

unsigned Cloth::getContraintSize() const
{
  return constraints.size();
}

std::vector<glm::vec4> &Cloth::getPositionData()
{
  return positionData;
}
std::vector<glm::vec4> &Cloth::getOldPositionData()
{
  return oldPositionData;
}
std::vector<glm::vec4> &Cloth::getAcelerationsData()
{
  return accelerationsData;
}
std::vector<glm::vec4> &Cloth::getConstraintsData()
{
  return constraintsData;
}
std::vector<glm::vec4> &Cloth::getConstraintsTwoData()
{
  return constraintsTwoData;
}

void Cloth::AddForce(glm::vec3 direction)
{
  for (auto &particle : particles)
  {
    particle.AddForce(direction); // add the forces to each particle
  }
}

void Cloth::AddForceToParticle_2(glm::vec3 force, unsigned width, unsigned height)
{
  unsigned particleNumber = width + (height * particlesWidthNumber);
  accelerationsData[particleNumber] += glm::vec4(force.x, force.y, force.z, 0);
}

void Cloth::SetForceToParticle_2(glm::vec3 force, unsigned width, unsigned height)
{
  unsigned particleNumber = width + (height * particlesWidthNumber);
  accelerationsData[particleNumber] = glm::vec4(force.x, force.y, force.z, 0);
}

void Cloth::Draw(Shader *shader, Transform &transform)
{

  glUseProgram(shader->shaderProgramID);

  DrawMode drawmode = DrawMode::EDefault;

  //  wireframe mode
  if (drawmode == DrawMode::EWireFrame)
  {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  }
  else
  {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  }

  unsigned int transformLoc = glGetUniformLocation(shader->shaderProgramID, "transform");
  glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform.getTransform()));
/*
  glBindVertexArray(VAO);
  glDrawElements(GL_TRIANGLES, indiciesSize, GL_UNSIGNED_INT, 0);
*/

  //glPointSize(10);
  glBindVertexArray(VAO);
  glDrawElements(GL_TRIANGLES, indiciesSize, GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}

Cloth::~Cloth()
{
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &EBO);
  printf("Cloth removed \n");
}