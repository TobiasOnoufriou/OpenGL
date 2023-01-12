#pragma once

#include "math_headers.h"

struct hit_record {
  glm::vec3 p;
  glm::vec3 normal;
  double t;
  bool front_face;

  inline void set_face_normal(glm::vec3 ray, glm::vec3& outward_normal) {
    //front_face = glm::dot(ray.)
  }


};


class Hittable {
public:
  virtual bool hit(double i);
};