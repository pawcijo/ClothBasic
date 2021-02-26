#include "Cloth/Constraint/Constraint.hpp"

Constraint::Constraint(Particle *particle1, Particle *particle2)
    : p1(particle1), p2(particle2) {

  glm::vec3 vector = p1->GetPosition() - p2->GetPosition();
  restDistance = sqrt(vector[0] * vector[0] + vector[1] * vector[1] +
                      vector[2] * vector[2]);
}

void Constraint::ResolveConstraint(int resolveNumber, float SpringConstant) {

  glm::vec3 p1_to_p2 = p2->GetPosition() - p1->GetPosition();

  float current_distance =
      sqrt(p1_to_p2[0] * p1_to_p2[0] + p1_to_p2[1] * p1_to_p2[1] +
           p1_to_p2[2] * p1_to_p2[2]);

  glm::vec3 correctionVector = p1_to_p2 * (1 - restDistance / current_distance);
  glm::vec3 correctionVectorHalf = correctionVector * 0.5f;

  float elasticy = 1.0 - pow(1.0 - SpringConstant, 1.0 / resolveNumber);

  p1->Move(elasticy * correctionVectorHalf);
  p2->Move(-(elasticy * correctionVectorHalf));
}

float Constraint::getRestDistance() { return restDistance; }