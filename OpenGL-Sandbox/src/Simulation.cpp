#pragma once
#include "Simulation.h"
#include <omp.h>

Simulation::Simulation() {
  std::vector<SparseMatrixTriplet> att_triplets;

  float av1 = 1;

  att_triplets.emplace_back(SparseMatrixTriplet(0, 0, av1));
  att_triplets.emplace_back(SparseMatrixTriplet(1, 1, av1));
  att_triplets.emplace_back(SparseMatrixTriplet(2, 2, av1));

  m_A_attachment.resize(3, 3);
  m_A_attachment.setFromTriplets(att_triplets.begin(), att_triplets.end());

  std::vector<SparseMatrixTriplet> spr_triplets;

  float sv1 = 0.5f;
  float sv2 = -0.5f;

  spr_triplets.emplace_back(SparseMatrixTriplet(0, 0, sv1));
  spr_triplets.emplace_back(SparseMatrixTriplet(1, 1, sv1));
  spr_triplets.emplace_back(SparseMatrixTriplet(2, 2, sv1));

  spr_triplets.emplace_back(SparseMatrixTriplet(0, 3, sv2));
  spr_triplets.emplace_back(SparseMatrixTriplet(1, 4, sv2));
  spr_triplets.emplace_back(SparseMatrixTriplet(2, 5, sv2));

  spr_triplets.emplace_back(SparseMatrixTriplet(3, 0, sv2));
  spr_triplets.emplace_back(SparseMatrixTriplet(4, 1, sv2));
  spr_triplets.emplace_back(SparseMatrixTriplet(5, 2, sv2));

  spr_triplets.emplace_back(SparseMatrixTriplet(3, 3, sv1));
  spr_triplets.emplace_back(SparseMatrixTriplet(4, 4, sv1));
  spr_triplets.emplace_back(SparseMatrixTriplet(5, 5, sv1));

  m_A_spring.resize(6, 6);
  m_A_spring.setFromTriplets(spr_triplets.begin(), spr_triplets.end());


  // Setting B sparse matrix
  m_B_attachment.resize(3, 3);
  m_B_attachment = m_A_attachment;
  
  m_B_spring.resize(6, 6);
  m_B_spring = m_A_spring;
}

SparseMatrix Simulation::CreateSMatrix(Constraint* c) {
  SparseMatrix s;
  std::vector<SparseMatrixTriplet> s_triplets;


  AttachmentConstraint* ac;
  if (ac = dynamic_cast<AttachmentConstraint*>(c)) {
    s_triplets.clear();
    unsigned int m_p0 = ac->GetConstraintType();
    s_triplets.emplace_back(SparseMatrixTriplet(0, m_p0 * 3 + 0, 1));
    s_triplets.emplace_back(SparseMatrixTriplet(1, m_p0 * 3 + 1, 1));
    s_triplets.emplace_back(SparseMatrixTriplet(2, m_p0 * 3 + 2, 1));
    s.resize(3, m_mesh->m_system_dimension);
  }
  SpringConstraint* sc;
  if (sc = dynamic_cast<SpringConstraint*>(c)) {
    s_triplets.clear();
    unsigned int m_p0 = sc->GetConstrainedVertexIndex1();
    s_triplets.emplace_back(SparseMatrixTriplet(0, m_p0 * 3 + 0, 1));
    s_triplets.emplace_back(SparseMatrixTriplet(1, m_p0 * 3 + 1, 1));
    s_triplets.emplace_back(SparseMatrixTriplet(2, m_p0 * 3 + 2, 1));
    unsigned int m_p1 = sc->GetConstrainedVertexIndex2();
    s_triplets.emplace_back(SparseMatrixTriplet(0, m_p0 * 3 + 0, 1));
    s_triplets.emplace_back(SparseMatrixTriplet(1, m_p0 * 3 + 1, 1));
    s_triplets.emplace_back(SparseMatrixTriplet(2, m_p0 * 3 + 2, 1));
  }

  s.setFromTriplets(s_triplets.begin(), s_triplets.end());

  return s;
}

void Simulation::CreateLHSMatrix() {
  SparseMatrix M; 
  M = m_mesh->m_mass_matrix / (m_h * m_h); // mesh->mass_matrix;
  for (std::vector<Constraint*>::iterator c = m_constraints.begin(); c != m_constraints.end(); ++c) {
    // Could this be moved above the for-loop. QUESTION FOR LATER.
    SparseMatrix S_i;
    SparseMatrix A_i;
    double w_i;

    AttachmentConstraint* ac;
    if (ac = dynamic_cast<AttachmentConstraint*>(*c)) {
      w_i = ac->Stiffness();
      A_i = m_A_attachment;
    }

    SpringConstraint* sc;
    if (sc = dynamic_cast<SpringConstraint*>(*c)) {
      w_i = sc->Stiffness();
      A_i = m_A_spring;
    }

    S_i = CreateSMatrix(*c);
    SparseMatrix S_i_transpose = S_i.transpose();
    SparseMatrix A_i_transpose = A_i;
    
    S_i_transpose.applyThisOnTheLeft(A_i_transpose);
    A_i_transpose.applyThisOnTheLeft(A_i);

    M += (w_i * S_i);
  }
  m_llt.compute(M);
}

void Simulation::CreateRHSMatrix() {
  for (std::vector<Constraint*>::iterator c = m_constraints.begin(); c != m_constraints.end(); ++c) {
    SparseMatrix A_i;
    SparseMatrix B_i;

    AttachmentConstraint* ac;
    if (ac = dynamic_cast<AttachmentConstraint*>(*c)) {
      A_i = m_A_attachment;
      B_i = m_B_attachment;
    }

    SpringConstraint* sc;
    if (sc = dynamic_cast<SpringConstraint*>(*c)) {
      A_i = m_A_spring;
      B_i = m_B_spring;
    }

    double w_i = (*c)->Stiffness();
    SparseMatrix S_i = CreateSMatrix((*c));
    SparseMatrix S_i_transpose = S_i.transpose();
    S_i_transpose.applyThisOnTheLeft(A_i);
    A_i.applyThisOnTheLeft(B_i);
    (*c)->m_RHS = w_i * B_i;
  }
}

void Simulation::Update() {
  calculateInertiaY();
  calculateExternalForce();

  AttachmentConstraint* ac;
  SpringConstraint* sc;

  VectorX* p_j;
  Constraint* c_j;

  unsigned int tn;

  VectorX q_n = m_mesh->m_current_positions;
  VectorX s_n = m_inertia_y + (m_h * m_h) * (m_mesh->inv_mass_matrix_) * m_external_force;
  VectorX q_n1 = s_n;

  SparseMatrix coeff = m_mesh->m_mass_matrix / (m_h * m_h);
  coeff.applyThisOnTheLeft(s_n);
  //Before simulation we'll need to get Spring and Attachment constraint values into an array.

  //After completion convert back to their original Eigenvalue
  // LOCAL SOLVE STEP
  for (int i = 0; i < 1; i++)
  {

    VectorX b = s_n;

    int rows = 0;

    EigenVector3 current_vector;
    ScalarType current_stretch;
    ConstraintType constraintType;

    int num_parallel_loops = ceil(m_constraints.size() / (float)omp_get_max_threads());
    for (int j = 0; j < num_parallel_loops; j++)
    {
      #pragma omp parallel default(shared) private(c_j, p_j, tn, sc, ac, tc, current_strecth, current_vector) 
      {
        tn = omp_get_thread_num() + j * omp_get_max_threads();
        if (tn < m_constraints.size())
        {
          c_j = m_constraints[tn];
          constraintType = c_j->GetConstraintType();

          #pragma omp critical 
          {
            if (constraintType == SpriConstraint) // is spring constraint
            {
              sc = (SpringConstraint*)c_j;
              current_vector = q_n1.block_vector(sc->GetConstrainedVertexIndex1()) - q_n1.block_vector(sc->GetConstrainedVertexIndex2());
              current_stretch = current_vector.norm() - sc->GetRestLength();
              current_vector = (current_stretch / 2.0) * current_vector.normalized();

              p_j = &p_spring; //Assign all the vectors from p_spring
              p_j->resize(6); //Resizes
              rows = 6;
              p_j->block_vector(0) = q_n1.block_vector(sc->GetConstrainedVertexIndex1()) - current_vector;
              p_j->block_vector(1) = q_n1.block_vector(sc->GetConstrainedVertexIndex2()) + current_vector;
            }

            else if (constraintType == AttConstraint) // is attachment constraint
            {
              ac = (AttachmentConstraint*)c_j;
              p_j = &p_attach;
              p_j->resize(3);

              p_j->block_vector(0) = ac->GetFixedPoint();
              rows = 3;
            }

            c_j->m_RHS.applyThisOnTheLeft(*p_j); //To enable working solution uncomment 
            b += *p_j;
          }
        }
      }
    }
    // GLOBAL SOLVE STEP
    q_n1 = m_llt.solve(b);
  }
  VectorX v_n1 = (q_n1 - q_n) / m_h;
  m_mesh->m_current_positions = q_n1;
  m_mesh->m_current_velocities = v_n1;

  // Add collision detection here using pos_next;
  //VectorX penetration = collisionDetection(m_mesh->m_current_positions);
  //m_mesh->m_current_positions -= penetration;

  // update velocity and damp
  dampVelocity();

}

void Simulation::calculateExternalForce() {
  m_external_force.resize(m_mesh->m_system_dimension);
  m_external_force.setZero();

  // gravity
  for (unsigned int i = 0; i < m_mesh->vertices_.size(); ++i)
  {
    m_external_force[3 * i + 1] += -m_gravity_constant;
  }

  m_external_force = m_mesh->m_mass_matrix * m_external_force;
}

void Simulation::calculateInertiaY() {
  m_inertia_y = m_mesh->m_current_positions + m_mesh->m_current_velocities * m_h;
}

void Simulation::dampVelocity() {
  m_mesh->m_current_velocities *= (1.0 - m_damping_coefficient);
}

void Simulation::setupConstraints() {
  clearConstraints();
  
  EigenVector3 pR1, pR2;
  for (std::vector<Edge>::iterator e = m_mesh->m_edge_list.begin(); e != m_mesh->m_edge_list.end(); ++e)
  {
    pR1 = m_mesh->m_current_positions.block_vector(e->m_v1);
    pR2 = m_mesh->m_current_positions.block_vector(e->m_v2);
    SpringConstraint* c = new SpringConstraint(&m_stiffness_stretch, e->m_v1, e->m_v2, (pR1 - pR2).norm());
    m_constraints.push_back(c);
  }

  // Setting up the bending constraints
  for (std::vector<Edge>::iterator e = m_mesh->m_edge_list.begin(); e != m_mesh->m_edge_list.end(); ++e)
  {
    pR1 = m_mesh->m_current_positions.block_vector(e->m_v1);
    pR2 = m_mesh->m_current_positions.block_vector(e->m_v2);
    SpringConstraint* c = new SpringConstraint(&m_stiffness_bending, e->m_v1, e->m_v2, (pR1 - pR2).norm());
    m_constraints.push_back(c);

  }

  AddAttachmentConstraint(0);
}

void Simulation::clearConstraints() {
  for (unsigned int i = 0; i < m_constraints.size(); ++i)
  {
    delete m_constraints[i];
  }
  m_constraints.clear();
}

Simulation::~Simulation() {
  clearConstraints();
}