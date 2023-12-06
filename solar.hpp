#ifndef SOLAR_HPP_
#define SOLAR_HPP_

#include "abcgOpenGL.hpp"
#include "planet.hpp"

struct Vertex {
  glm::vec3 position{};
  glm::vec3 normal{};
  glm::vec2 texCoord{};
  
  friend bool operator==(Vertex const &, Vertex const &) = default;
};

class Body {
public:
  void create(GLuint program); // cria a esfera e os buffers
  void paint();
  void destroy(); // destroi os buffers já utilizados
  void update(float deltaTime,float rot_speed, float trans_speed); 
  void computeModelMatrix();
  void render(glm::mat4 viewMatrix) const;
  void generateUVSphere(int stacks, int slices);

  std::string name{""};
  std::vector<Vertex> m_vertices;
  std::vector<GLuint> m_indices;
  std::vector<GLuint> m_lines_indices;

  GLuint m_VAO{};
  GLuint m_VBO{};
  GLuint m_EBO{};
  
  GLuint m_program{};

  glm::vec3 m_lightPos{0.0f, 0.0f, 0.0f};
  glm::vec4 m_Ka{0.5f};
  glm::vec4 m_Kd{0.5f};
  glm::vec4 m_Ks{0.5f};


  std::string texture_path{""};
  std::string normal_map_path{""};
  std::string specular_map_path{""};
  std::string cloud_map_path{""};
  std::string night_map_path{""};

  GLuint m_diffuseTexture{};
  GLuint m_diffuseTexture_night{};
  GLuint m_normal_map{};
  GLuint m_specular_map{};
  GLuint m_clouds_map{};

  glm::vec4 color{};
  float scale{1.0f};
  Body *satellite_of;

  Path path;

  glm::vec3 position{0.0f};
  float orbit_radius{};
  float translation_angle{0.0f};
  float translation_speed{0.0f};
  float rotation_angle{0.0f};
  float rotation_speed{1.0f};

  glm::mat4 modelMatrix{1.0f};

  GLint m_modelMatrixLoc{};
  GLint m_colorLoc{};
  
};

#endif