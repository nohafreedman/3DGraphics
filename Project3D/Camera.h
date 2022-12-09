#pragma once
#include <glm/mat4x4.hpp>
class Camera
{
public:
	float m_theta;
	float m_phi;
	float m_speed = 1;
	float speedMod = 1;
	glm::vec3 m_position;
	Camera();
	Camera(glm::vec3 pos);
	Camera(float theta, float phi, float speed, glm::vec3 pos);
	glm::mat4 Camera::getViewMatrix();
	glm::mat4 Camera::getProjectionMatrix(float w, float h);
	void update(float deltaTime);
};

