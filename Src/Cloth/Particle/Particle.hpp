#pragma once
#include <glm/glm.hpp>
#include <memory>
#include "Transform/Transform.hpp"

class Particle {

  bool moveable;
  float mass;

  glm::vec3 position;
  glm::vec3 oldPosition;
  glm::vec3 acceleration;
  glm::vec3 accumulatedNormal;
  
  //Debug
  Transform transform;
 
public:
  Particle(glm::vec3 pos);
  Particle() = default;


  const Transform & GetTransformRef() const;
  Transform * GetTransformPtr();

  void AddForce(glm::vec3 force);
  void SetForce(glm::vec3 force);

  /*
  Function Update - old timeStep
  force = mass * accleration,
  calculates position and sets old position
  */
  void Update(float elapsedTime);
  glm::vec3 &GetPosition(); // TODO change to Transform
  glm::vec3 &GetAcceleration();
  void ResetAcceleration();

  /*
    Function Move  - old offsetPos
    pos+= vector;
  */
  void Move(const glm::vec3 vector);

  /*
    Function SetMoveable  - old make Unmoveable
  */
  void SetMoveable(bool value);

  bool GetMoveable();

  void AddToNormal(glm::vec3 normal);

  glm::vec3 &GetNormal();

  void ResetNormal();
};