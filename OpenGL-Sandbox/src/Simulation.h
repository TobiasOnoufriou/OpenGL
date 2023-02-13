#pragma once
#include "math_headers.h"
#include "Mesh.hpp"
#include "Constraint.h"
#include <omp.h>
#include <vector>

class Simulation {
public:
  Simulation();
  ~Simulation();

  // Single Simulation Per Scene.
  inline void  SetMesh(Mesh* mesh) { m_mesh = mesh; }

  void Update();

  void CreateLHSMatrix(void);
  void CreateRHSMatrix(void);
  SparseMatrix CreateSMatrix(Constraint* c);

protected:

  double m_h; // Time Step.

  double m_gravity_constant;
  double m_stiffness_attachment;
  double m_stiffness_stretch;
  double m_stiffness_bending;
  double m_damping_coefficient;
  Eigen::LLT<Matrix> m_llt;

  std::vector<Constraint*> m_constraints;

  unsigned int m_itterations_per_frame;

  VectorX m_inertia_y;
  VectorX m_external_force;

  
  // Attachment and Spring Constraint sparse matricies
  SparseMatrix m_A_attachment;
  SparseMatrix m_A_spring;
  SparseMatrix m_B_attachment;
  SparseMatrix m_B_spring;

  VectorX p_attach;
  VectorX p_spring;

  // Mesh
  Mesh* m_mesh;
private: 
  void calculateInertiaY();
  void calculateExternalForce();
  VectorX collisionDetection(const VectorX x) {};
  void dampVelocity();
  void setupConstraints();
  void clearConstraints();
  void AddAttachmentConstraint(unsigned int vertex_index) {
    AttachmentConstraint* ac = new AttachmentConstraint(&m_stiffness_attachment, vertex_index, m_mesh->m_current_positions.block_vector(vertex_index));
    m_constraints.push_back(ac);
    //m_mesh->m_expanded_system_dimension += 3;
    //m_mesh->m_expanded_system_dimension_1d += 1;
    CreateLHSMatrix();
    CreateRHSMatrix();
  }
};