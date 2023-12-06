#ifndef SPACE_HPP_
#define SPACE_HPP_

#include "abcgOpenGL.hpp"

struct VertexDome {
  glm::vec3 position{};
  glm::vec3 normal{};
  glm::vec2 texCoord{};
  
  friend bool operator==(VertexDome const &, VertexDome const &) = default;
};

class Skydome {
public:
  void create(GLuint program);
  void paint();
  void destroy();
  void computeModelMatrix();
  void render(glm::mat4 viewMatrix, glm::mat4 projMatrix) const;
  void generateUVSphere(int stacks, int slices);

  std::vector<VertexDome> m_vertices;
  std::vector<GLuint> m_indices;

  GLuint m_VAO{};
  GLuint m_VBO{};
  GLuint m_EBO{};
  
  GLuint m_sky_program{};

  std::string texture_path{""};
  GLuint m_diffuseTexture{};

  glm::vec4 color{};
  float scale{1.0f};


  glm::vec3 position{0.0f};
  glm::mat4 modelMatrix{1.0f};

  GLint m_modelMatrixLoc{};
  GLint m_colorLoc{};

  
};

#endif