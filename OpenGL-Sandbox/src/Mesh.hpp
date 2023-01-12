// Copyright 2022 Tobias Onoufriou

#pragma once
#include "GLCore.h"
#include <GLCoreUtils.h>


#include "math_headers.h"
#include "hittable.h"
#include "BoxCollider.h"

#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <vector>
#include <math.h>

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
  bool intersectionTest(glm::vec3 origin, glm::vec3 dir) {
    //float tmin = -INFINITY, tmax = INFINITY;
    float dir_inv = 1.0f / dir.x;


    float t1 = (pMin.x - origin.x) * dir_inv;
    float t2 = (pMax.x - origin.x) * dir_inv;

    float tmin = std::min(t1, t2);
    float tmax = std::max(t1, t2);


    for (auto i = 1; i < 3; ++i) {
      dir_inv = 1.0f / dir[i];
      t1 = (pMin[i] - origin[i]) * dir_inv;
      t2 = (pMax[i] - origin[i]) * dir_inv;


      tmin = std::max(tmin, std::min(t1, t2));
      tmax = std::max(tmax, std::min(t1, t2));
    }
    return tmax > std::max(tmin, 0.0f) ? true : false;

  }
  void TranslateShape(glm::vec3 translation);
  void SetupMesh();
  virtual void CalculateBoundingVolume() override;
  virtual void DrawBoundingBox(GLCore::Utils::PerspectiveCameraController cam) override;


  bool GetCollider() { return collider; }

 private:
  std::vector<Vertex> vertices_;
  std::vector<GLuint> indices_;
  std::vector<Texture> textures_;

  VectorX current_positions_;
  VectorX current_velocities_;
  SparseMatrix mass_matrix_;
  SparseMatrix inv_mass_matrix_;
  SparseMatrix identity_matrix_;

  SparseMatrix mass_matrix_1d_;
  SparseMatrix identity_matrix_1d_;

  unsigned int system_dimension_;

  //Scale and Position
  glm::vec3 Scale_, Position_;
  glm::mat4 model;
  
  // Mesh/Material Shader.
  GLCore::Utils::Shader* shader_;
  GLCore::Utils::Shader* debug_shader_;

  GLuint vbo_, ebo_, vao_;
};
