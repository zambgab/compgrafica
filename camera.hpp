#ifndef CAMERA_HPP_
#define CAMERA_HPP_

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include "abcg.hpp"
#include "solar.hpp"

class Camera {
public:
  void mouseMove(glm::ivec2 const &position);
  void mousePress(glm::ivec2 const &position);
  void mouseRelease(glm::ivec2 const &position);
  void resizeViewport(glm::ivec2 const &size);
  void mouseScroll(float scroll);
  void trackBody(Body body);

  // [[nodiscard]] glm::mat4 getRotation() const;

  void computeViewMatrix();
  void computeProjectionMatrix(glm::vec2 const &size);

  void pedestal(float speed);
  void truck(float speed);

  glm::mat4 const &getViewMatrix() const { return m_viewMatrix; }
  glm::mat4 const &getProjMatrix() const { return m_projMatrix; }

  bool isTrackingBody{false};
private:
  glm::vec3 m_eye{0.0f, 10.0f, 10.0f}; // Camera position
  glm::vec3 m_at{0.0f, 0.0f, 0.0f};  // Look-at point
  glm::vec3 m_up{0.0f, 1.0f, 0.0f};  // "up" direction

  glm::vec3 m_axis{1.0f}; // axis for rotating camera when mouse is dragged
  glm::mat4 m_rotation{1.0f}; // angle for rotating camera when mouse is dragged 

  glm::vec3 tracking_vector{2.0f};

  bool m_mouseTracking{};

  glm::vec3 m_lastPosition{};

  glm::ivec2 m_viewportSize{};

  // Matrix to change from world space to camera space
  glm::mat4 m_viewMatrix;

  // Matrix to change from camera space to clip space
  glm::mat4 m_projMatrix;

  [[nodiscard]] glm::vec3 project(glm::vec2 const &mousePosition) const;
};

#endif