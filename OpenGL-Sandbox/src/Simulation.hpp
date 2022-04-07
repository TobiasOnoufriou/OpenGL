#pragma once
#include "Constraint.hpp"
#include "Mesh.hpp"

class Simulation {
public:
  Simulation();
  virtual ~Simulation();

  void Reset();
  void Update();

  void CreateLHSMatrix(void);
  void CreateRHSMatrix(void);
  SparseMatrix CreateSMatrix(Constraint* c);


private:
  void ClearConstraints();
  void SetupConstraints();

};

#pragma once
#include "Simulation.hpp"

Simulation::Simulation() {

}

void Simulation::ClearConstraint() {

}

void Simulation::SetupConstraint() {

}
