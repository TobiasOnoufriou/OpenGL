// Copyright 2022 Tobias Onoufriou

#pragma once
#include "GLCore.h"

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
  void SetupMesh();
 private:
  std::vector<Vertex> vertices_;
  std::vector<GLuint> indices_;
  std::vector<Texture> textures_;
  GLuint vbo_, ebo_, vao_; //I think the ebo_ is the ibo_
};
