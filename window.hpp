#ifndef WINDOW_HPP_
#define WINDOW_HPP_

#include "abcgOpenGL.hpp"
#include "solar.hpp"
#include "camera.hpp"
#include "space.hpp"

class Window : public abcg::OpenGLWindow {
protected:
  void onEvent(SDL_Event const &event) override;
  void onCreate() override;
  void onUpdate() override;
  void onPaint() override;
  void onPaintUI() override;
  void onResize(glm::ivec2 const &size) override;
  void onDestroy() override;

private:
  glm::ivec2 m_viewportSize{};

  Camera m_camera;

  Body sun, mercury, venus, earth, mars, moon;

  Skydome skydome;

  float m_pedestalSpeed{};
  float m_truckSpeed{};
  
  float m_rotation_speed{0.0f};
  float m_translation_speed{0.0f};

  struct Light
  {
    glm::vec4 m_Ia{1.0f};
    glm::vec4 m_Id{1.0f};
    glm::vec4 m_Is{1.0f};
  };

  Light light;

  float m_Ia{1.0f};
  float m_Id{1.0f};
  float m_Is{1.0f};
  float m_shininess{50.0f};

  int radio_selected{-1};
  
  ImFont *m_font{};

  GLuint program_body{}, program_path{}, program_skydome{};
};

#endif
