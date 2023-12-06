#include "planet.hpp"


void Path::create(GLuint program){
  generateCircle(300);

  m_program = program;

  // Delete previous buffers
  abcg::glDeleteBuffers(1, &m_VBO);

  // VBO
  abcg::glGenBuffers(1, &m_VBO);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
  abcg::glBufferData(GL_ARRAY_BUFFER,
                     sizeof(m_vertices.at(0)) * m_vertices.size(),
                     m_vertices.data(), GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  
  m_modelMatrixLoc = abcg::glGetUniformLocation(program, "modelMatrix");
  m_colorLoc = abcg::glGetUniformLocation(program, "color");


  // Release previous VAO
  abcg::glDeleteVertexArrays(1, &m_VAO);

  // Create VAO
  abcg::glGenVertexArrays(1, &m_VAO);
  abcg::glBindVertexArray(m_VAO);

  // Bind EBO and VBO
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

  // Bind vertex attributes
  auto const positionAttribute{
      abcg::glGetAttribLocation(program, "inPosition")};
  if (positionAttribute >= 0) {
    abcg::glEnableVertexAttribArray(positionAttribute);
    abcg::glVertexAttribPointer(positionAttribute, 3, GL_FLOAT, GL_FALSE,
                                sizeof(glm::vec3), nullptr);
  }

  // End of binding
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);
  abcg::glBindVertexArray(0);



  computeModelMatrix();


}

void Path::destroy(){
  abcg::glDeleteBuffers(1, &m_VBO);
  abcg::glDeleteVertexArrays(1, &m_VAO);
}

void Path::update(glm::vec3 pos){
  position = pos;

  computeModelMatrix();  
}
void Path::computeModelMatrix(){
  modelMatrix = glm::mat4(1.0f);
  modelMatrix = glm::translate(modelMatrix, position);
}

void Path::generateCircle(int num_vertices){
  float x,z;
  float angle_step = 2 * M_PI / (num_vertices);
  for(auto i : iter::range(num_vertices)){
    auto vertice_angle = i * angle_step;
    x = std::cos(vertice_angle) * orbit_radius;
    z = std::sin(vertice_angle) * orbit_radius;
    m_vertices.push_back(glm::vec3(x,0.0f,z));
  }
}

void Path::render() const {

  abcg::glUseProgram(m_program);
  // Set uniform variables for the current model
  abcg::glUniformMatrix4fv(m_modelMatrixLoc, 1, GL_FALSE, &modelMatrix[0][0]);
  abcg::glUniform4fv(m_colorLoc, 1, &color[0]); 

  abcg::glBindVertexArray(m_VAO);

  glm::vec4 line_color = {0.0f,0.0f,0.0f,0.0f} ;
  abcg::glUniform4fv(m_colorLoc, 1, &line_color[0]);
  abcg::glDrawArrays(GL_LINES,0,m_vertices.size());

  abcg::glBindVertexArray(0);
}
