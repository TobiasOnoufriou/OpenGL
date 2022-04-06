// Copyright 2022 Tobias Onoufriou

#include "Mesh.hpp"
#include "math_headers.h"

Mesh::Mesh(std::vector<Vertex> vertices,
  std::vector<GLuint> indices,
  std::vector<Texture> textures) {
  this->vertices_ = vertices;
  this->indices_ = indices;
  this->textures_ = textures;

}

Mesh::~Mesh() {}

void Mesh::ChangeVertexPosition(glm::vec3 pos, int index) {
  current_positions_.block_vector(index) += GLM2Eigen(pos);
}

//Generation for 3D meshes.
void Mesh::GenerateParticleList(unsigned int system_dimension, unsigned int vertex) {
  system_dimension_ = system_dimension;

  current_positions_.resize(system_dimension_);
  current_velocities_.resize(system_dimension_);
  mass_matrix_.resize(system_dimension_, system_dimension_);
  inv_mass_matrix_.resize(system_dimension_, system_dimension_);
  identity_matrix_.resize(system_dimension_, system_dimension_);

  mass_matrix_1d_.resize(vertex,vertex);
  identity_matrix_1d_.resize(vertex, vertex);

  current_positions_.setZero();

  for (unsigned int index = 0; index < vertices_.size(); ++index) {
    current_positions_.block_vector(index) = GLM2Eigen(vertices_[index].position);
  }

  current_velocities_.setZero();

  // Assign mass matrix and an equally sized identity matrix
  std::vector<SparseMatrixTriplet> i_triplets;
  std::vector<SparseMatrixTriplet> m_triplets;
  std::vector<SparseMatrixTriplet> m_inv_triplets;
  i_triplets.clear();
  m_triplets.clear();
  ScalarType inv_unit_mass = 1.0 / 1.0f;
  for (unsigned int index = 0; index < system_dimension_; index++)
  {
    i_triplets.push_back(SparseMatrixTriplet(index, index, 1));
    m_triplets.push_back(SparseMatrixTriplet(index, index, 1.0f));
    m_inv_triplets.push_back(SparseMatrixTriplet(index, index, inv_unit_mass));
  }
  mass_matrix_.setFromTriplets(m_triplets.begin(), m_triplets.end());
  inv_mass_matrix_.setFromTriplets(m_inv_triplets.begin(), m_inv_triplets.end());
  identity_matrix_.setFromTriplets(i_triplets.begin(), i_triplets.end());
  m_triplets.clear();
  i_triplets.clear();
  for (unsigned int index = 0; index < vertex; index++)
  {
    i_triplets.push_back(SparseMatrixTriplet(index, index, 1));
    m_triplets.push_back(SparseMatrixTriplet(index, index, 1.0f));
  }
  mass_matrix_1d_.setFromTriplets(m_triplets.begin(), m_triplets.end());
  identity_matrix_1d_.setFromTriplets(i_triplets.begin(), i_triplets.end());
  
}

void Mesh::SetupMesh() {
  if(!glIsBuffer(vao_))
    glGenVertexArrays(1, &vao_);
  if (!glIsBuffer(vbo_))
    glGenBuffers(1, &vbo_);
  if (!glIsBuffer(ebo_))
    glGenBuffers(1, &ebo_);

  glBindVertexArray(vao_);


  std::cout << vertices_.at(1).position.x << std::endl;
  for (unsigned int i = 0; i < vertices_.size(); ++i) {
    vertices_[i].position = glm::vec3(current_positions_[3 * i + 0], current_positions_[3 * i + 1], current_positions_[3 * i + 2]);
  }

  glBindBuffer(GL_ARRAY_BUFFER, vbo_);
  glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(Vertex), &vertices_[0], GL_DYNAMIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_.size() * sizeof(unsigned int), &indices_[0], GL_STATIC_DRAW);

  // vertex position
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
  // vertex normals
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
  // vertex texture coords
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tex_coords));
  // vertex tangent
  glEnableVertexAttribArray(3);
  glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));
  // vertex bitangent
  glEnableVertexAttribArray(4);
  glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, bitangent));

  glBindVertexArray(0);
}

// Draw will be added to the update function.
// Implementation of Shader is needed.
void Mesh::Draw(GLuint shaderId) {
  // get uniform location.
          // bind appropriate textures
  unsigned int diffuseNr = 1;
  unsigned int specularNr = 1;
  unsigned int normalNr = 1;
  unsigned int heightNr = 1;

  SetupMesh();

  for (unsigned int i = 0; i < textures_.size(); i++)
  {
    glActiveTexture(GL_TEXTURE0 + i);
    std::string number;
    std::string name = textures_[i].type;
    if (name == "texture_diffuse")
      number = std::to_string(diffuseNr++);
    else if (name == "texture_specular")
      number = std::to_string(specularNr++);
    else if (name == "texture_normal")
      number = std::to_string(normalNr++);
    else if (name == "texture_height")
      number = std::to_string(heightNr++);

    glUniform1i(glGetUniformLocation(shaderId, (name + number).c_str()), i);
    glBindTexture(GL_TEXTURE_2D, textures_[i].id);
  }

  // draw mesh
  glBindVertexArray(vao_);
  glDrawElements(GL_TRIANGLES, indices_.size(), GL_UNSIGNED_INT, 0);
  
  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
  glDisableVertexAttribArray(2);
  glDisableVertexAttribArray(3);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  // always good practice to set everything back to defaults once configured.
  glActiveTexture(GL_TEXTURE0);

}


