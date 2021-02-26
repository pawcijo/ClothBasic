
#pragma once

#include "Cloth/Particle/Particle.hpp"

class Constraint {

  float restDistance; // rest distance between particle P1 and P2

public:
  Particle *p1 = nullptr;
  Particle *p2 = nullptr;

  Constraint(Particle *p1, Particle *p2);

  void ResolveConstraint(int resolveNumber,float SpringConstant);
  float getRestDistance();
};
