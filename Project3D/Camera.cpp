#include "Camera.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <Input.h>

Camera::Camera()
{
    m_phi = 0;
    m_theta = 0;
    m_speed = 0;
    m_position = glm::vec3(0, 0, 0);
}

Camera::Camera(glm::vec3 pos)
{
    m_phi = 0;
    m_theta = 0;
    m_speed = 5;
    m_position = pos;
}

Camera::Camera(float theta, float phi, float speed, glm::vec3 pos)
{
    m_theta = theta;
    m_phi = phi;
    m_speed = speed;
    m_position = pos;
}

glm::mat4 Camera::getViewMatrix()
{
    // really strange way of doing camera stuff but I like it
    float thetaR = glm::radians(m_theta);
    float phiR = glm::radians(m_phi);
    // finds a forward vector from the angles the camera is SUPPOSED to have
    glm::vec3 forward(cos(phiR) * cos(thetaR), sin(phiR), cos(phiR) * sin(thetaR));
    // uses openGL math to point the camera along the forward vector we found
    return glm::lookAt(m_position, m_position + forward, glm::vec3(0, 1, 0));
}

glm::mat4 Camera::getProjectionMatrix(float w, float h)
{
    // calculates a perspective matrix with specified field of view, aspect ratio, and near/far plane values
    return glm::perspective(glm::pi<float>() * 0.25f, w / h, 0.1f, 1000.f);
}

void Camera::update(float deltaTime)
{
    aie::Input* input = aie::Input::getInstance();

    // Get camera local coords and rotation
    float thetaR = glm::radians(m_theta);
    float phiR = glm::radians(m_phi);

    // get camera relative vectors
    glm::vec3 forward(cos(phiR) * cos(thetaR), sin(phiR), cos(phiR) * sin(thetaR));
    glm::vec3 right(-sin(thetaR), 0, cos(thetaR));
    glm::vec3 up(0, 1, 0);

    // mouse rotation

    float mx = input->getMouseDeltaX();
    float my = input->getMouseDeltaY();
    const float turnSpeed = 0.1f;

    if (input->isMouseButtonDown(aie::INPUT_MOUSE_BUTTON_RIGHT)) {
        m_theta += turnSpeed * mx;
        m_phi -= turnSpeed * -my;
    }

    // classic input stuff
    // speedMod changes from 1 to 2 if SHIFT is pressed
    if (input->isKeyDown(aie::INPUT_KEY_W))
        m_position += forward * m_speed * speedMod * deltaTime;
    if(input->isKeyDown(aie::INPUT_KEY_S))
        m_position -= forward * m_speed * speedMod * deltaTime;
    if (input->isKeyDown(aie::INPUT_KEY_A))
        m_position -= right * m_speed * speedMod * deltaTime;
    if (input->isKeyDown(aie::INPUT_KEY_D))
        m_position += right * m_speed * speedMod * deltaTime;
    if (input->isKeyDown(aie::INPUT_KEY_C))
        m_position -= up * m_speed * speedMod * deltaTime;
    if (input->isKeyDown(aie::INPUT_KEY_SPACE))
        m_position += up * m_speed * speedMod * deltaTime;

    if (input->isKeyDown(aie::INPUT_KEY_LEFT_SHIFT))
        speedMod = 2;
    else
        speedMod = 1;


}
