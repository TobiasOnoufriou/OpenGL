// Copyright 2022 Tobias Onoufriou

#pragma once
#include "GLCore.h"
#include <GLCoreUtils.h>


#include "hittable.h"
#include "BoxCollider.h"

#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <vector>
#include <math.h>

#define minI(x, y) ((x) < (y) ? (x) : (y))
#define maxI(x, y) ((x) > (y) ? (x) : (y))

struct Vertex {
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec2 tex_coords;
  glm::vec3 tangent;
  glm::vec3 bitangent;
};

struct Texture {
  unsigned int id;
  std::string type;
  std::string path;
};

struct Edge {
  unsigned int m_v1, m_v2; // indices of endpoint vertices
  unsigned int m_tri1, m_tri2; // indices of adjacent faces
};

// Mesh would inherit a collider and material

class Mesh: public BoxCollider {
public:
  Mesh(
    std::vector<Vertex> vertices,
    std::vector<GLuint> indices,
    std::vector<Texture> textures
  );
  ~Mesh();

  void Draw(GLCore::Utils::PerspectiveCameraController camera);
  void ChangeVertexPosition(glm::vec3 pos, int index);
  void GenerateParticleList(unsigned int system_dimension, unsigned int vertex);
  void GenerateEdgeList();
  bool intersectionTest(glm::vec3 origin, glm::vec3 dir) {
    float tmin = 0.0, tmax = INFINITY;
    
    for (int i = 0; i < 3; ++i) {
      float t1 = (pMin[i],- origin[i]) * (dir[i]);
      float t2 = (pMax[i] - origin[i]) * (dir[i]);

      tmin = maxI(tmin, minI(t1, t2));
      tmax = minI(tmax, maxI(t1, t2));
    }

    return tmin < tmax;

  }
  void TranslateShape(glm::vec3 translation);
  void SetupMesh();
  void ComputeNormal();
  virtual void CalculateBoundingVolume() override;
  virtual void DrawBoundingBox(GLCore::Utils::PerspectiveCameraController cam) override;


  bool GetCollider() { return collider; }

  unsigned int m_system_dimension;


 public:
  std::vector<Vertex> vertices_;
  std::vector<GLuint> indices_;
  std::vector<Texture> textures_;

  VectorX m_current_positions;
  VectorX m_current_velocities;
  SparseMatrix m_mass_matrix;
  SparseMatrix inv_mass_matrix_;
  SparseMatrix identity_matrix_;

  SparseMatrix mass_matrix_1d_;
  SparseMatrix identity_matrix_1d_;

  std::vector<Edge> m_edge_list;

  //Scale and Position
  glm::vec3 Scale_, Position_;
  glm::mat4 model;
  
  // Mesh/Material Shader.
  GLCore::Utils::Shader* shader_;
  GLCore::Utils::Shader* debug_shader_;

  GLuint vbo_, ebo_, vao_;
};
