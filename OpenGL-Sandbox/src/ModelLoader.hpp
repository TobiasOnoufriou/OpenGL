// Copyright 2022 Tobias Onoufriou
#pragma once
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/cimport.h>
#include "stb_image/stb_image.h"

#include <vector>
#include <string>

#include "Mesh.hpp"
//#include <assimp/.hpp>


class ModelLoader {
 public:
  ModelLoader(std::string const& path, bool gamma);
  ~ModelLoader();
  bool LoadModel(std::string const& path);
  void ProcessNode(aiNode* node, const aiScene* kScene);
  Mesh ProcessMesh(aiMesh* mesh, const aiScene* kScene);
  void ExpandModel();
  std::vector<Texture> LoadMaterialTextures(
     aiMaterial* mat,
     aiTextureType type,
     std::string typeName);
  unsigned int TextureFromFile(const std::string& path, const std::string& directory, bool gamma);

  void Draw(GLuint shader_id);
 private:
  std::vector<Mesh> meshes_;
  std::vector<Texture> textures_loaded_;
  std::string directory_;
  bool gamme_correction_;
};
