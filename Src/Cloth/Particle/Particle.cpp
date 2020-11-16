#include "Cloth/Particle/Particle.hpp"

#include "Transform/Transform.hpp"

const float DampingLenght = 0.01f;

Particle::Particle(glm::vec3 aPosition, float aMass)
    : position(aPosition), oldPosition(aPosition),
      acceleration(glm::vec3(0, 0, 0)), mass(aMass), moveable(true),
      transform() {}

void Particle::AddForce(const glm::vec3 force)
{
  acceleration += force / mass;
}

void Particle::SetForce(const glm::vec3 force)
{
  acceleration = force / mass;
}

void Particle::Update(float elapsedTime)
{

  if (moveable)
  {
    glm::vec3 temporary = position;
    position = position + (position - oldPosition) * (1.0f - DampingLenght) +
               acceleration * elapsedTime;
    oldPosition = temporary;
    acceleration = glm::vec3(0, 0, 0);
  }
}

glm::vec3 &Particle::GetAcceleration() { return acceleration; }

glm::vec3 &Particle::GetPosition() { return position; }

glm::vec3 Particle::GetOldPositionCopy()
{
  return oldPosition;
}
glm::vec3 Particle::GetPositionCopy()
{
  return position;
}
glm::vec3 Particle::GetAccelerationCopy()
{
  return acceleration;
}

void Particle::ResetAcceleration() { acceleration = glm::vec3(0, 0, 0); }

void Particle::Move(const glm::vec3 aPosition)
{
  if (moveable)
  {
    position += aPosition;
  }
  transform.setPosition(position.x, position.y, position.z);
}

void Particle::SetMoveable(bool value) { moveable = value; }

bool Particle::GetMoveable() { return moveable; }

const Transform &Particle::GetTransformRef() const
{
  return transform;
}

 bool Particle::isMoveable()
 {
   return moveable;
 }

 bool Particle::operator==(const Particle &rhs) const
 {
   return (position == rhs.position && 
           acceleration == rhs.acceleration &&
           oldPosition == rhs.oldPosition &&
           moveable == rhs.moveable &&
           mass == rhs.mass);
 }


Transform *Particle::GetTransformPtr()
{
  return &transform;
}
