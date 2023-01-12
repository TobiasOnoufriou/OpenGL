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
  ModelLoader();
  ModelLoader(std::string const& path, bool gamma);
  ~ModelLoader();
  std::unique_ptr<Mesh>& LoadModel(std::string const& path);
  void ProcessNode(aiNode* node, const aiScene* kScene);
  void ProcessMesh(aiMesh* mesh, const aiScene* kScene);

  template<typename Vec>
  std::pair<Vec,Vec>GetExtents(const Vec* pts, size_t stride, size_t count);

  template<typename Vec>
  void CenterAndScale(Vec* pts, size_t stride, size_t count, const typename Vec::value_type& size);


  std::vector<Texture> LoadMaterialTextures(
     aiMaterial* mat,
     aiTextureType type,
     std::string typeName);
  unsigned int TextureFromFile(const std::string& path, const std::string& directory, bool gamma);
  const aiScene* getScene() { return kScene; }
  std::unique_ptr<Mesh> meshes_;
 private:
  std::vector<Texture> textures_loaded_;
  std::string directory_;
  const aiScene* kScene;
  bool gamme_correction_;
};
