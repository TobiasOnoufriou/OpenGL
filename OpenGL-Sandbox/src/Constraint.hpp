#pragma once
#include "math_headers.h"
#include "Mesh.hpp"

enum {ATTACHMENT, SPRING};

class Constraint {
public:
  Constraint(ScalarType* stifness);
  Constraint(const Constraint& other);
  virtual ~Constraint();

  virtual ScalarType EvaluatePotentialEnergy(const VectorX& x) { return 0; };

  inline const ScalarType& Stiffness() { return (*stiffness_); };

  int constraint_type_;
protected:
  ScalarType* stiffness_;
public:
  virtual void Draw(const Mesh& mesh) {}; //Not necessary yet. But would be useful to have user feedback.
};

class AttachmentConstraint : public Constraint {
public:
  AttachmentConstraint(ScalarType* stiffness);
  AttachmentConstraint(ScalarType* stiffness, unsigned int p0, const EigenVector3& fixedpoint);
  AttachmentConstraint(const AttachmentConstraint& other);
  virtual ~AttachmentConstraint();
  virtual ScalarType EvaluatePotentialEnergy(const VectorX& x);
protected:
  unsigned int m_p0;
  EigenVector3 m_g;
  EigenVector3 m_fixd_point;

  bool m_selected;

public:
  virtual void Draw(const Mesh& vbos);
  inline void Select() { m_selected = true; }
  inline void UnSelect() { m_selected = false; }
  inline EigenVector3 GetFixedPoint() { return m_fixd_point; }
  inline void SetFixedPoint(const EigenVector3& target) { m_fixd_point = target; }
  inline unsigned int GetConstrainedVertexIndex() { return m_p0; }
};

class SpringConstraint : public Constraint {
public:
  SpringConstraint(ScalarType* stiffness);
  SpringConstraint(ScalarType* stiffness, unsigned int p1, unsigned int p2, ScalarType length);
  SpringConstraint(const SpringConstraint& other);
  virtual ~SpringConstraint();

  virtual ScalarType  EvaluatePotentialEnergy(const VectorX& x);
protected:
  unsigned int m_p1, m_p2;
  EigenVector3 m_g1, m_g2;
  // rest length
  ScalarType m_rest_length;

public:
  inline ScalarType GetRestLength(void) { return m_rest_length; }
  inline unsigned int GetConstrainedVertexIndex1(void) { return m_p1; }
  inline unsigned int GetConstrainedVertexIndex2(void) { return m_p2; }
};