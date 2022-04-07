#include "Constraint.hpp"

Constraint::Constraint(ScalarType* stiffness):
  stiffness_(stiffness) {

}

Constraint::Constraint(const Constraint& other) :
  stiffness_(other.stiffness_) {
}

Constraint::~Constraint() {
}

AttachmentConstraint::AttachmentConstraint(ScalarType* stiffness) :
  Constraint(stiffness)
{
  m_selected = false;
  constraint_type_ = ATTACHMENT;
}

AttachmentConstraint::AttachmentConstraint(ScalarType* stiffness, unsigned int p0, const EigenVector3& fixedpoint) :
  Constraint(stiffness),
  m_p0(p0),
  m_fixd_point(fixedpoint)
{
  m_selected = false;
  constraint_type_ = ATTACHMENT;
}

AttachmentConstraint::AttachmentConstraint(const AttachmentConstraint& other) :
  Constraint(other),
  m_p0(other.m_p0),
  m_fixd_point(other.m_fixd_point),
  m_selected(other.m_selected)
{
  constraint_type_ = ATTACHMENT;
}

AttachmentConstraint::~AttachmentConstraint()
{

}

ScalarType AttachmentConstraint::EvaluatePotentialEnergy(const VectorX& x)
{
  ScalarType k = *stiffness_;
  EigenVector3 current_position = x.block_vector(m_p0);
  ScalarType current_length = (current_position - m_fixd_point).norm();
  return 0.5 * k * current_length * current_length * 10;
}

SpringConstraint::SpringConstraint(ScalarType* stiffness) :
  Constraint(stiffness)
{
  constraint_type_ = SPRING;
}

SpringConstraint::SpringConstraint(ScalarType* stiffness, unsigned int p1, unsigned int p2, ScalarType length) :
  Constraint(stiffness),
  m_p1(p1),
  m_p2(p2),
  m_rest_length(length)
{
  constraint_type_ = SPRING;
}

SpringConstraint::SpringConstraint(const SpringConstraint& other) :
  Constraint(other),
  m_p1(other.m_p1),
  m_p2(other.m_p2),
  m_rest_length(other.m_rest_length)
{
  constraint_type_ = SPRING;
}

SpringConstraint::~SpringConstraint()
{
}

ScalarType SpringConstraint::EvaluatePotentialEnergy(const VectorX& x)
{
  ScalarType k = *stiffness_;
  EigenVector3 current_position_p1 = x.block_vector(m_p1);
  EigenVector3 current_position_p2 = x.block_vector(m_p2);
  ScalarType current_length = (current_position_p1 - current_position_p2).norm();
  ScalarType difference = current_length - m_rest_length;
  return 0.5 * k * difference * difference;
}
