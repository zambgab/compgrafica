#include "solar.hpp"
#include <filesystem>

// Representa o corpo no sistema solar com sua posição e

void Body::create(GLuint program){
  generateUVSphere(36,36);
  m_program = program;
  // Delete previous buffers
  abcg::glDeleteBuffers(1, &m_EBO);
  abcg::glDeleteBuffers(1, &m_VBO);

  // VBO
  abcg::glGenBuffers(1, &m_VBO);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
  abcg::glBufferData(GL_ARRAY_BUFFER,
                     sizeof(m_vertices.at(0)) * m_vertices.size(),
                     m_vertices.data(), GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // EBO
  abcg::glGenBuffers(1, &m_EBO);
  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
  abcg::glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                     sizeof(m_indices.at(0)) * m_indices.size(),
                     m_indices.data(), GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  
  m_modelMatrixLoc = abcg::glGetUniformLocation(program, "modelMatrix");
  m_colorLoc = abcg::glGetUniformLocation(program, "color");


  // Release previous VAO
  abcg::glDeleteVertexArrays(1, &m_VAO);

  // Create VAO
  abcg::glGenVertexArrays(1, &m_VAO);
  abcg::glBindVertexArray(m_VAO);

  // Bind EBO and VBO
  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

  // Bind vertex attributes
  auto const positionAttribute{abcg::glGetAttribLocation(program, "inPosition")};
  if (positionAttribute >= 0) {
    abcg::glEnableVertexAttribArray(positionAttribute);
    abcg::glVertexAttribPointer(positionAttribute, 3, GL_FLOAT, GL_FALSE,
                                sizeof(Vertex), nullptr);
  }

  auto const normalAttribute{abcg::glGetAttribLocation(program, "inNormal")};
  if (normalAttribute >= 0) {
    abcg::glEnableVertexAttribArray(normalAttribute);
    auto const offset{offsetof(Vertex, normal)};
    abcg::glVertexAttribPointer(normalAttribute, 3, GL_FLOAT, GL_FALSE,
                                sizeof(Vertex),
                                reinterpret_cast<void *>(offset));
  }

  auto const texCoordAttribute{
      abcg::glGetAttribLocation(program, "inTexCoord")};
  if (texCoordAttribute >= 0) {
    abcg::glEnableVertexAttribArray(texCoordAttribute);
    auto const offset{offsetof(Vertex, texCoord)};
    abcg::glVertexAttribPointer(texCoordAttribute, 2, GL_FLOAT, GL_FALSE,
                                sizeof(Vertex),
                                reinterpret_cast<void *>(offset));
  }

  // End of binding
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);
  abcg::glBindVertexArray(0);

  if (std::filesystem::exists(texture_path)){
    abcg::glDeleteTextures(1, &m_diffuseTexture);
    m_diffuseTexture = abcg::loadOpenGLTexture({.path = texture_path});
  }
  if (std::filesystem::exists(night_map_path)){
    abcg::glDeleteTextures(1, &m_diffuseTexture_night);
    m_diffuseTexture_night = abcg::loadOpenGLTexture({.path = night_map_path});
  }
  

  if (std::filesystem::exists(cloud_map_path)){
    abcg::glDeleteTextures(1, &m_clouds_map);
    m_clouds_map = abcg::loadOpenGLTexture({.path = cloud_map_path});
  }
  if (std::filesystem::exists(specular_map_path)){
    abcg::glDeleteTextures(1, &m_specular_map);
    m_specular_map = abcg::loadOpenGLTexture({.path = specular_map_path});
  }

  if (satellite_of){
    position = satellite_of->position + glm::vec3{orbit_radius, 0.0f, 0.0f};
  }

  computeModelMatrix();
  path.orbit_radius = orbit_radius;
}

void Body::destroy(){
  abcg::glDeleteTextures(1, &m_diffuseTexture);
  abcg::glDeleteBuffers(1, &m_EBO);
  abcg::glDeleteBuffers(1, &m_VBO);
  abcg::glDeleteVertexArrays(1, &m_VAO);
}

void Body::update(float deltaTime, float rot_speed, float trans_speed){
  // updates position for bodies that are satellites of another body
  if (satellite_of){
    // translation
    auto angle = 5.0f * deltaTime * translation_speed * trans_speed;
    translation_angle += angle;
    position.x = satellite_of->position.x + cos(translation_angle) * orbit_radius;
    position.z = satellite_of->position.z - sin(translation_angle) * orbit_radius;

    //rotation
    angle = 5.0f * deltaTime * rotation_speed * rot_speed;
    rotation_angle += angle;
  
    path.update(satellite_of->position);
  }
  computeModelMatrix();
  
}
void Body::computeModelMatrix(){
  modelMatrix = glm::mat4(1.0f);
  modelMatrix = glm::translate(modelMatrix, position);
  modelMatrix = glm::rotate(modelMatrix, rotation_angle,glm::vec3(0.0f,1.0f,0.0f));
  modelMatrix = glm::scale(modelMatrix, glm::vec3(scale));
}

void Body::generateUVSphere(int stacks, int sectors){
  // geneterates a UV sphere with radius 1.0f
  float sectorAngle, stackAngle, x, y, z, xz, u, v;
  float sectorStep = 2 * M_PI / sectors;
  float stackStep = M_PI / stacks;

  for(auto i : iter::range(stacks+1)){
    stackAngle = M_PI / 2 - i * stackStep;
    xz = std::cos(stackAngle);
    y = std::sin(stackAngle);

    for(auto j: iter::range(sectors+1)){
      sectorAngle = j * sectorStep;

      x = xz * std::sin(sectorAngle);
      z = xz * std::cos(sectorAngle);


      u = (float) j / sectors;
      v = 1.0f - (float) i / stacks;

      Vertex const vertex{.position = {x, y, z}, .normal = {x, y, z}, .texCoord = {u,v}};

      m_vertices.push_back(vertex);

    }
  }
  // now we create the indices for the triangles
  int k1, k2;
  for(int i = 0; i < stacks; ++i){
    k1 = i * (sectors + 1);     // beginning of current stack
    k2 = k1 + sectors + 1;      // beginning of next stack

    for(int j = 0; j < sectors; ++j, ++k1, ++k2)
    {
      // 2 triangles per sector excluding first and last stacks
      // k1 => k2 => k1+1
      if(i != 0)
      {
          m_indices.push_back(k1);
          m_indices.push_back(k2);
          m_indices.push_back(k1 + 1);
      }

      // k1+1 => k2 => k2+1
      if(i != (stacks-1))
      {
          m_indices.push_back(k1 + 1);
          m_indices.push_back(k2);
          m_indices.push_back(k2 + 1);
      }

      // store indices for lines
      // vertical lines for all stacks, k1 => k2
      m_lines_indices.push_back(k1);
      m_lines_indices.push_back(k2);
      if(i != 0)  // horizontal lines except 1st stack, k1 => k+1
      {
          m_lines_indices.push_back(k1);
          m_lines_indices.push_back(k1 + 1);
      }
    }
  }


}

void Body::render(glm::mat4 viewMatrix) const {
  // Set uniform variables for the current model
  abcg::glUniformMatrix4fv(m_modelMatrixLoc, 1, GL_FALSE, &modelMatrix[0][0]);
  abcg::glUniform4fv(m_colorLoc, 1, &color[0]); 

    auto const normalMatrixLoc{
      abcg::glGetUniformLocation(m_program, "normalMatrix")};
  auto const lightPosLoc{
      abcg::glGetUniformLocation(m_program, "lightPosWorldSpace")};

  auto const KaLoc{abcg::glGetUniformLocation(m_program, "Ka")};
  auto const KdLoc{abcg::glGetUniformLocation(m_program, "Kd")};
  auto const KsLoc{abcg::glGetUniformLocation(m_program, "Ks")};

  auto const diffuseTexLoc{abcg::glGetUniformLocation(m_program, "diffuseTex")};
  auto const diffuseTexNightLoc{abcg::glGetUniformLocation(m_program, "diffuseTexNight")};
  // auto const normalTexLoc{abcg::glGetUniformLocation(m_program, "normalTex")};
  auto const specularTexLoc{abcg::glGetUniformLocation(m_program, "specularTex")};
  auto const cloudsTexLoc{abcg::glGetUniformLocation(m_program, "cloudsTex")};
  
  abcg::glUniform3fv(lightPosLoc, 1, &m_lightPos.x);

  abcg::glUniform1i(diffuseTexLoc, 0);
  abcg::glUniform1i(diffuseTexNightLoc, 1);
  abcg::glUniform1i(specularTexLoc, 2);
  abcg::glUniform1i(cloudsTexLoc, 3);

  auto const modelViewMatrix{glm::mat3(viewMatrix * modelMatrix)};
  auto const normalMatrix{glm::inverseTranspose(modelViewMatrix)};
  abcg::glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, &normalMatrix[0][0]);

  abcg::glUniform4fv(KaLoc, 1, &m_Ka.x);
  abcg::glUniform4fv(KdLoc, 1, &m_Kd.x);
  abcg::glUniform4fv(KsLoc, 1, &m_Ks.x);

  abcg::glBindVertexArray(m_VAO);

  abcg::glActiveTexture(GL_TEXTURE0);
  abcg::glBindTexture(GL_TEXTURE_2D, m_diffuseTexture);
  
  auto const hasExtraTexLoc{abcg::glGetUniformLocation(m_program, "hasExtraTex")};
  abcg::glUniform1i(hasExtraTexLoc, false);

  if (name == "Earth"){

    abcg::glUniform1i(hasExtraTexLoc, true);
    
    abcg::glActiveTexture(GL_TEXTURE1);
    abcg::glBindTexture(GL_TEXTURE_2D, m_diffuseTexture_night);

    abcg::glActiveTexture(GL_TEXTURE2);
    abcg::glBindTexture(GL_TEXTURE_2D, m_specular_map);

    abcg::glActiveTexture(GL_TEXTURE3);
    abcg::glBindTexture(GL_TEXTURE_2D, m_clouds_map);
  }

  // Set minification and magnification parameters
  abcg::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  abcg::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // Set texture wrapping parameters
  abcg::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  abcg::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  // Draw body triangles
  abcg::glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, nullptr);

  abcg::glBindVertexArray(0);
}


