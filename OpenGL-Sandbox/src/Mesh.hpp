// Copyright 2022 Tobias Onoufriou

#pragma once
#include "GLCore.h"

#include "math_headers.h"

#include <string>
#include <vector>

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

class Mesh{
 public:
  Mesh(
    std::vector<Vertex> vertices,
    std::vector<GLuint> indices,
    std::vector<Texture> textures
    );
  ~Mesh();
  void Draw(GLuint shaderId);
  void ChangeVertexPosition(glm::vec3 pos, int index);
  void GenerateParticleList(unsigned int system_dimension, unsigned int vertex);
  void SetupMesh();
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

  GLuint vbo_, ebo_, vao_;
};
