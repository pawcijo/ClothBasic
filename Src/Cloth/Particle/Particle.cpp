#include "Cloth/Particle/Particle.hpp"

#include "Transform/Transform.hpp"

const float DampingLenght = 0.01f;

Particle::Particle(glm::vec3 aPosition)
    : position(aPosition), oldPosition(aPosition),
      acceleration(glm::vec3(0, 0, 0)), mass(1.f), moveable(true),
      accumulatedNormal(glm::vec3(0, 0, 0)),
      transform() {}

void Particle::AddForce(const glm::vec3 force) {
    acceleration += force / mass;
}

void Particle::SetForce(const glm::vec3 force)
{
  acceleration = force / mass;
}

void Particle::Update(float elapsedTime) {

  if (moveable) {
    glm::vec3 temporary = position;
    position = position + (position - oldPosition) * (1.0f - DampingLenght) +
               acceleration * elapsedTime;
    oldPosition = temporary;
    acceleration = glm::vec3(0,0,0);
  }

}

glm::vec3 &Particle::GetAcceleration() { return acceleration; }

glm::vec3 &Particle::GetPosition() { return position; }

void Particle::ResetAcceleration() { acceleration = glm::vec3(0, 0, 0); }

void Particle::Move(const glm::vec3 aPosition) {
  if (moveable) {
    position += aPosition;
  }
  transform.setPosition(position.x,position.y,position.z);
}

void Particle::SetMoveable(bool value) { moveable = value; }

void Particle::AddToNormal(glm::vec3 normal) {
  accumulatedNormal += glm::normalize(normal);
}

glm::vec3 &Particle::GetNormal() { return accumulatedNormal; }

void Particle::ResetNormal() { accumulatedNormal = glm::vec3(0, 0, 0); }

bool Particle::GetMoveable() { return moveable; }

const Transform & Particle::GetTransformRef()const
{
  return transform;
}

Transform * Particle::GetTransformPtr()
{
  return &transform;
}
