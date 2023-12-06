#ifndef PLANET_HPP_
#define PLANET_HPP_

#include "abcgOpenGL.hpp"

class Path {
public:
  void create(GLuint program);
  void paint();
  void destroy();
  void update(glm::vec3 pos);
  void computeModelMatrix();
  void render() const;
  void generateCircle(int num_vertices);

  float orbit_radius{};

private:
  GLuint m_VAO{};
  GLuint m_VBO{};

  std::vector<glm::vec3> m_vertices;
  std::vector<GLuint> m_indices;

  glm::vec3 position{0.0f};

  glm::vec4 color{};

  glm::mat4 modelMatrix{1.0f};

  GLint m_modelMatrixLoc{};
  
  GLint m_colorLoc{};

  GLint m_program{};

  
};

#endif
