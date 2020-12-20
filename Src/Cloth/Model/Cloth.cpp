
#include "Cloth/Model/Cloth.hpp"

#include <iostream>
#include <algorithm>
#include "Src/Util/ClothGeneration/ClothGeneration.hpp"

#include "Src/Util/DrawMode/DrawMode.hpp"

Particle *Cloth::GetParticle(int x, int y)
{
  return &CPUparticles[y * particlesWidthNumber + x];
}

void Cloth::MakeConstriant(Particle *p1, Particle *p2)
{
  CPUconstraints.push_back(Constraint(p1, p2));
}

void Cloth::generateBuffers(unsigned particleNumber, unsigned constraintNumber)
{

  indicies = genereteIndicies(std::pair<unsigned, unsigned>(particlesWidthNumber, particlesHeightNumber));
  indiciesSize = indicies.size() * sizeof(unsigned);

  glGenBuffers(1, &positionVbo);
  glGenBuffers(1, &oldPositionSSbo);
  glGenBuffers(1, &accelerationSSbo);
  glGenBuffers(1, &constraintSSbo);
  glGenBuffers(1, &EBO);

  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, positionVbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * positionData.size(), &positionData[0], GL_STATIC_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, positionVbo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicies.size() * sizeof(unsigned), &indicies[0], GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (char *)0 + 0 * sizeof(GLfloat));
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, positionVbo);

  glBindBuffer(GL_SHADER_STORAGE_BUFFER, oldPositionSSbo);
  glBufferData(GL_SHADER_STORAGE_BUFFER, oldPositionData.size() * sizeof(glm::vec4), oldPositionData.data(), GL_STREAM_COPY);
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, oldPositionSSbo);

  glBindBuffer(GL_SHADER_STORAGE_BUFFER, accelerationSSbo);
  glBufferData(GL_SHADER_STORAGE_BUFFER, accelerationsData.size() * sizeof(glm::vec4), accelerationsData.data(), GL_STREAM_COPY);
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, accelerationSSbo);

  glBindBuffer(GL_SHADER_STORAGE_BUFFER, constraintSSbo);
  glBufferData(GL_SHADER_STORAGE_BUFFER, constraintsData.size() * sizeof(glm::vec4), constraintsData.data(), GL_STREAM_COPY);
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, constraintSSbo);

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
}

Cloth::Cloth(float width, float height, unsigned particleWidth,
             unsigned particleHeight)
    : particlesWidthNumber(particleWidth),
      particlesHeightNumber(particleWidth)
{

  CPUparticles.resize(particleWidth *
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

      CPUparticles[y * particlesWidthNumber + x] =
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

  for (int i = 0; i < CPUparticles.size(); i++)
  {
    positionData.push_back(glm::vec4(CPUparticles[i].GetPositionCopy(),
                                     CPUparticles[i].isMoveable()));
  }

  for (auto particle : CPUparticles)
  {
    oldPositionData.push_back(glm::vec4(particle.GetOldPositionCopy(), 0));
  }

  for (auto particle : CPUparticles)
  {
    accelerationsData.push_back(glm::vec4(particle.GetAccelerationCopy(), 0));
  }

  // GPU CONSTRAINTS
  for (int x = 0; x < particlesWidthNumber; x++)
  {
    for (int y = 0; y < particlesHeightNumber; y++)
    {

      if (x < particlesWidthNumber - 1)
      {
         Constraint constraint = Constraint(GetParticle(x, y), GetParticle(x + 1, y));

        constraintsData.push_back(glm::vec4(constraint.p1->getIndex(),
                                            constraint.p2->getIndex(), constraint.getRestDistance(), 0));
        //MakeConstriant(GetParticle(x, y), GetParticle(x + 1, y));
      }

      if (y < particlesHeightNumber - 1)
      {
         Constraint constraint = Constraint(GetParticle(x, y), GetParticle(x, y + 1));

        constraintsData.push_back(glm::vec4(constraint.p1->getIndex(),
                                            constraint.p2->getIndex(), constraint.getRestDistance(), 0));
        //MakeConstriant(GetParticle(x, y), GetParticle(x, y + 1));
      }

      if (x < particlesWidthNumber - 1 && y < particlesHeightNumber - 1)
      {
         Constraint constraint = Constraint(GetParticle(x, y), GetParticle(x + 1, y + 1));

        constraintsData.push_back(glm::vec4(constraint.p1->getIndex(),
                                            constraint.p2->getIndex(), constraint.getRestDistance(), 0));
        //MakeConstriant(GetParticle(x, y), GetParticle(x + 1, y + 1));
      }

      if (x < particlesWidthNumber - 1 && y < particlesHeightNumber - 1)
      {
         Constraint constraint = Constraint(GetParticle(x + 1, y), GetParticle(x, y + 1));

        constraintsData.push_back(glm::vec4(constraint.p1->getIndex(),
                                            constraint.p2->getIndex(), constraint.getRestDistance(), 0));
        //MakeConstriant(GetParticle(x + 1, y), GetParticle(x, y + 1));
      }
    }
  }

  for (int x = 0; x < particlesWidthNumber; x++)
  {
    for (int y = 0; y < particlesHeightNumber; y++)
    {
      if (x < particlesWidthNumber - 2)
      {

        Constraint constraint = Constraint(GetParticle(x, y), GetParticle(x + 2, y));

        constraintsData.push_back(glm::vec4(constraint.p1->getIndex(),
                                            constraint.p2->getIndex(), constraint.getRestDistance(), 0));
      }
      //MakeConstriant(GetParticle(x, y), GetParticle(x + 2, y));
      if (y < particlesHeightNumber - 2)
      {
        Constraint constraint = Constraint(GetParticle(x, y), GetParticle(x, y + 2));
        constraintsData.push_back(glm::vec4(constraint.p1->getIndex(),
                                            constraint.p2->getIndex(), constraint.getRestDistance(), 0));

        //MakeConstriant(GetParticle(x, y), GetParticle(x, y + 2));
      }
      if (x < particlesWidthNumber - 2 && y < particlesHeightNumber - 2)
      {
        Constraint constraint = Constraint(GetParticle(x, y), GetParticle(x + 2, y + 2));

        constraintsData.push_back(glm::vec4(constraint.p1->getIndex(),
                                            constraint.p2->getIndex(), constraint.getRestDistance(), 0));

        //MakeConstriant(GetParticle(x, y), GetParticle(x + 2, y + 2));
      }
      if (x < particlesWidthNumber - 2 && y < particlesHeightNumber - 2)
      {
        Constraint constraint = Constraint(GetParticle(x + 2, y), GetParticle(x, y + 2));

        constraintsData.push_back(glm::vec4(constraint.p1->getIndex(),
                                            constraint.p2->getIndex(), constraint.getRestDistance(), 0));

        //MakeConstriant(GetParticle(x + 2, y), GetParticle(x, y + 2));
      }
    }
  }


  std::cout << "\n";
  std::cout << "Cloth buffers generation test:"
            << "\n";
  std::cout << "PositionData Size :" << positionData.size() << "\n";
  std::cout << "OldPositionData Size :" << oldPositionData.size() << "\n";
  std::cout << "AccelerationData Size :" << accelerationsData.size() << "\n";
  std::cout << "ConstraintsData Size :" << constraintsData.size() << "\n";
  std::cout << "\n";

  generateBuffers(particlesWidthNumber * particlesHeightNumber, CPUconstraints.size());
}

std::vector<Particle> &Cloth::GetParticles() { return CPUparticles; }

void Cloth::Update(float elapsedTime, int CONSTRAINT_ITERATIONS)
{

  for (int i = 0; i < CONSTRAINT_ITERATIONS; i++) // iterate over all constraints several times
  {
    for (auto &constraint : CPUconstraints)
    {
      constraint.ResolveConstraint(); // satisfy constraint.
    }
  }

  for (auto &particle : CPUparticles)
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
  return static_cast<unsigned>(CPUparticles.size());
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
  return CPUconstraints.size();
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

void Cloth::AddForce(glm::vec3 direction)
{
  for (auto &particle : CPUparticles)
  {
    particle.AddForce(direction); // add the forces to each particle
  }
}

  void Cloth::AddForceGPU(glm::vec3 force,std::vector<int> indicies)
  {
    for(auto indic : indicies)
    {
         accelerationsData[indic] += glm::vec4(force.x, force.y, force.z, 0);
    }
      glBindBuffer(GL_SHADER_STORAGE_BUFFER, accelerationSSbo);
  glBufferData(GL_SHADER_STORAGE_BUFFER, accelerationsData.size() * sizeof(glm::vec4), accelerationsData.data(), GL_STREAM_COPY);
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, accelerationSSbo);
  }
  void Cloth::AddForceGPU(glm::vec3 force)
  {
    for(int i =0;i<accelerationsData.size();i++)
    {
        accelerationsData[i] += glm::vec4(force.x, force.y, force.z, 0);
    }
      glBindBuffer(GL_SHADER_STORAGE_BUFFER, accelerationSSbo);
  glBufferData(GL_SHADER_STORAGE_BUFFER, accelerationsData.size() * sizeof(glm::vec4), accelerationsData.data(), GL_STREAM_COPY);
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, accelerationSSbo);
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

  DrawMode drawmode = DrawMode::EWireFrame;

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