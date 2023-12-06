#include "window.hpp"

void Window::onEvent(SDL_Event const &event) {
  glm::ivec2 mousePosition;
  SDL_GetMouseState(&mousePosition.x, &mousePosition.y);

  if (event.type == SDL_KEYDOWN) {
    if (event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_w)
      m_pedestalSpeed = 1.0f;
    if (event.key.keysym.sym == SDLK_DOWN || event.key.keysym.sym == SDLK_s)
      m_pedestalSpeed = -1.0f;
    if (event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_a)
      m_truckSpeed = -1.0f;
    if (event.key.keysym.sym == SDLK_RIGHT || event.key.keysym.sym == SDLK_d)
      m_truckSpeed = 1.0f;
  }
  if (event.type == SDL_KEYUP) {
    if ((event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_w) && m_pedestalSpeed > 0)
      m_pedestalSpeed = 0.0f;
    if ((event.key.keysym.sym == SDLK_DOWN || event.key.keysym.sym == SDLK_s) && m_pedestalSpeed < 0)
      m_pedestalSpeed = 0.0f;
    if ((event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_a) && m_truckSpeed < 0)
      m_truckSpeed = 0.0f;
    if ((event.key.keysym.sym == SDLK_RIGHT || event.key.keysym.sym == SDLK_d) && m_truckSpeed > 0)
      m_truckSpeed = 0.0f;
  }
  if (event.type == SDL_MOUSEMOTION) {
    m_camera.mouseMove(mousePosition);
  }
  if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
    m_camera.mousePress(mousePosition);
  }
  if (event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT) {
    m_camera.mouseRelease(mousePosition);
  }

  if (event.type == SDL_MOUSEWHEEL) {
    m_camera.mouseScroll(event.wheel.preciseY);
  }

}

void Window::onCreate() {
  auto const assetsPath{abcg::Application::getAssetsPath()};

  auto const filename{assetsPath + "fonts/nasalization.ttf"};
  m_font = ImGui::GetIO().Fonts->AddFontFromFileTTF(filename.c_str(), 20.0f);

  abcg::glClearColor(0, 0, 0, 1);
  abcg::glEnable(GL_DEPTH_TEST);
  

  auto const path{assetsPath + "shaders/"};
  program_body =
      abcg::createOpenGLProgram({{.source = path + "body.vert",
                                  .stage = abcg::ShaderStage::Vertex},
                                 {.source = path + "body.frag",
                                  .stage = abcg::ShaderStage::Fragment}});

  program_path =
      abcg::createOpenGLProgram({{.source = path + "path.vert",
                                  .stage = abcg::ShaderStage::Vertex},
                                 {.source = path + "path.frag",
                                  .stage = abcg::ShaderStage::Fragment}});

  program_skydome =
      abcg::createOpenGLProgram({{.source = path + "skydome.vert",
                                  .stage = abcg::ShaderStage::Vertex},
                                 {.source = path + "skydome.frag",
                                  .stage = abcg::ShaderStage::Fragment}});

  // Sol
  sun.scale = 1.0f;
  sun.texture_path = assetsPath + "maps/sol.jpg";
  sun.m_Ka = glm::vec4(1.0f);
  sun.satellite_of = nullptr;
  sun.create(program_body);

  // Mercúrio
  mercury.scale = 0.1f;
  mercury.texture_path = assetsPath + "maps/mercurio.jpg";
  mercury.translation_speed = 1.0f/88.0f;
  mercury.rotation_speed = -1.0f/59.0f;
  mercury.orbit_radius = 1.5f;
  mercury.satellite_of = &sun;  
  mercury.create(program_body);

  // Venus
  venus.scale = 0.1f;
  venus.texture_path = assetsPath + "maps/venus.jpg";
  venus.translation_speed = 1.0f/224.0f;
  venus.rotation_speed = 1.0f/243.0f;
  venus.orbit_radius = 2.0f;
  venus.satellite_of = &sun;
  venus.create(program_body);

  // Terra
  earth.scale = 0.1f;
  earth.texture_path = assetsPath + "maps/8k_earth_daymap.jpg";
  earth.night_map_path = assetsPath + "maps/8k_earth_nightmap.jpg";
  earth.specular_map_path = assetsPath + "maps/8k_earth_specular_map.jpg";
  earth.cloud_map_path = assetsPath + "maps/8k_earth_clouds.jpg";
  earth.translation_speed = 1.0f/365.0f;
  earth.rotation_speed = 1.0f;
  earth.orbit_radius = 2.5f;
  earth.satellite_of = &sun;
  earth.create(program_body);

  // Marte
  mars.scale = 0.05f;
  mars.texture_path = assetsPath + "maps/marte.jpg";
  mars.translation_speed = 1.0f/686.0f;
  mars.rotation_speed = 1.0f/1.03f;
  mars.orbit_radius = 3.0f;
  mars.satellite_of = &sun;
  mars.create(program_body);
  
  // Lua
  moon.scale = 0.02f;
  moon.texture_path = assetsPath + "maps/lua.jpg";
  moon.translation_speed = 1.0f/27.3220f;
  moon.rotation_speed = 1.0f/27.0f;
  moon.orbit_radius = 0.2f;
  moon.satellite_of = &earth;
  moon.create(program_body);
  
  mercury.path.create(program_path);
  venus.path.create(program_path);
  earth.path.create(program_path);
  mars.path.create(program_path);
  moon.path.create(program_path);

  // create skydome
  skydome.texture_path = assetsPath + "negx.jpg";
  skydome.create(program_skydome);

}

void Window::onUpdate() {
  auto const deltaTime{gsl::narrow_cast<float>(getDeltaTime())};
  
  sun.update(deltaTime,m_rotation_speed,m_translation_speed);
  mercury.update(deltaTime,m_rotation_speed,m_translation_speed);
  venus.update(deltaTime,m_rotation_speed,m_translation_speed);
  earth.update(deltaTime,m_rotation_speed,m_translation_speed);
  mars.update(deltaTime,m_rotation_speed,m_translation_speed);
  moon.update(deltaTime,m_rotation_speed,m_translation_speed);

  // Update LookAt camera
  m_camera.pedestal(m_pedestalSpeed * deltaTime);
  m_camera.truck(m_truckSpeed * deltaTime);
}

void Window::onPaint() {
  abcg::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  abcg::glEnable(GL_BLEND);
  abcg::glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  abcg::glViewport(0, 0, m_viewportSize.x, m_viewportSize.y);

  abcg::glUseProgram(program_body);

  // Get location of uniform variables
  auto viewMatrixLoc{abcg::glGetUniformLocation(program_body, "viewMatrix")};
  auto projMatrixLoc{abcg::glGetUniformLocation(program_body, "projMatrix")};

  // Set uniform variables that have the same value for every model
  abcg::glUniformMatrix4fv(viewMatrixLoc, 1, GL_FALSE, &m_camera.getViewMatrix()[0][0]);
  abcg::glUniformMatrix4fv(projMatrixLoc, 1, GL_FALSE, &m_camera.getProjMatrix()[0][0]);

  auto const IaLoc{abcg::glGetUniformLocation(program_body, "Ia")};
  auto const IdLoc{abcg::glGetUniformLocation(program_body, "Id")};
  auto const IsLoc{abcg::glGetUniformLocation(program_body, "Is")};
  auto const shininessLoc{abcg::glGetUniformLocation(program_body, "shininess")};

  abcg::glUniform4fv(IaLoc, 1, &light.m_Ia.x);
  abcg::glUniform4fv(IdLoc, 1, &light.m_Id.x);
  abcg::glUniform4fv(IsLoc, 1, &light.m_Is.x);
  abcg::glUniform1f(shininessLoc, m_shininess);

  sun.render(m_camera.getViewMatrix());
  mercury.render(m_camera.getViewMatrix());
  venus.render(m_camera.getViewMatrix());
  earth.render(m_camera.getViewMatrix());
  mars.render(m_camera.getViewMatrix());
  moon.render(m_camera.getViewMatrix());

  abcg::glUseProgram(0);

  abcg::glUseProgram(program_path);

  // Get location of uniform variables
  viewMatrixLoc = abcg::glGetUniformLocation(program_path, "viewMatrix");
  projMatrixLoc = abcg::glGetUniformLocation(program_path, "projMatrix");

  // Set uniform variables that have the same value for every model
  abcg::glUniformMatrix4fv(viewMatrixLoc, 1, GL_FALSE, &m_camera.getViewMatrix()[0][0]);
  abcg::glUniformMatrix4fv(projMatrixLoc, 1, GL_FALSE, &m_camera.getProjMatrix()[0][0]);

  mercury.path.render();
  venus.path.render();
  earth.path.render();
  mars.path.render();
  moon.path.render();


  abcg::glUseProgram(0);


  skydome.render(m_camera.getViewMatrix(), m_camera.getProjMatrix());
}

void Window::onPaintUI() {
  abcg::OpenGLWindow::onPaintUI();
  
  {
    ImGui::PushFont(m_font);
  }
  
  ImGui::PopFont();

}

void Window::onResize(glm::ivec2 const &size) {
  m_viewportSize = size;
  m_camera.resizeViewport(size);
  m_camera.computeProjectionMatrix(size);
}

void Window::onDestroy() {
  abcg::glDeleteProgram(program_body);
}
