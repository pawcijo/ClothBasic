#pragma once
#include <glm/glm.hpp>
#include <memory>
#include "Transform/Transform.hpp"

class Particle {

  bool moveable;
  float mass;

  int index;
  int xPosition,yPosition;

  glm::vec3 position;
  glm::vec3 oldPosition;
  glm::vec3 acceleration;

  //Debug
  Transform transform;

public:
  Particle(glm::vec3 pos,int index,int xPosition,int yPosition, float mass = 1.0);
  Particle() = default;

  bool operator==(const Particle &rhs) const;


  const Transform & GetTransformRef() const;
  Transform * GetTransformPtr();

  void AddForce(glm::vec3 force);
  void SetForce(glm::vec3 force);

  void Update(float elapsedTime);
  glm::vec3 &GetPosition();
  glm::vec3 &GetAcceleration();
  void ResetAcceleration();

  glm::vec3 GetOldPositionCopy();
  glm::vec3 GetPositionCopy();
  glm::vec3 GetAccelerationCopy();

  std::pair<int,int> getXYPosition();
  int getIndex();

  bool isMoveable();
  void Move(const glm::vec3 vector);


  void SetMoveable(bool value);

  bool GetMoveable();

  void AddToNormal(glm::vec3 normal);

  glm::vec3 &GetNormal();

  void ResetNormal();
};
