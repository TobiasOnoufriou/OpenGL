#pragma once
#include <glm/glm/glm.hpp>

enum ConstraintType {
  AttConstraint,
  SpriConstraint,
};

class Constraint {
public:
  Constraint() {};
  virtual ~Constraint() {};
  ConstraintType GetConstraintType() { return m_ctype; }
  double Stiffness() { return m_stiffness; }

  SparseMatrix m_RHS;

protected:
  ConstraintType m_ctype; // Constraint Type
  double m_stiffness;
};

class AttachmentConstraint: public Constraint {
public:
  // FixedPoint and shouldn't be updated
  AttachmentConstraint(ScalarType* stiffness) {};
  AttachmentConstraint(ScalarType* stiffness, unsigned int p0, const EigenVector3& fixedpoint) {};
  AttachmentConstraint(const AttachmentConstraint& other) {};
  virtual ~AttachmentConstraint() {};

  inline unsigned int GetConstrainedVertexIndex() { return m_p0; };
  inline EigenVector3 GetFixedPoint() { return m_fixed_point; }
  inline void SetFixedPoint(const EigenVector3& target) { m_fixed_point = target; }
protected:
  unsigned int m_p0;
  ConstraintType type = AttConstraint;
  EigenVector3 m_g;
  EigenVector3 m_fixed_point;
};

class SpringConstraint: public Constraint {
public:
  SpringConstraint(ScalarType *stiffness) {};
  SpringConstraint(ScalarType* stifness, unsigned int p0, unsigned int p1, ScalarType length) {};
  SpringConstraint(const SpringConstraint& other) {};
  virtual ~SpringConstraint() {};
  // Rest length
  // F = -kx  
  // Add collision normals
protected: 


 //Setting the type to do comparison
public:
  inline double GetRestLength() const& { return m_rest_length; }
  inline unsigned int GetConstrainedVertexIndex1(void) { return m_p0; };
  inline unsigned int GetConstrainedVertexIndex2(void) { return m_p1; };
private:
  unsigned int m_p0, m_p1;
  EigenVector3 m_g0, m_g1;

  ScalarType m_rest_length;
};