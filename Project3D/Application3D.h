#pragma once

#include "Application.h"
#include "Shader.h"
#include "Mesh.h"
#include "OBJMesh.h"
#include "Camera.h"
#include "Scene.h"
#include <glm/mat4x4.hpp>

class Application3D : public aie::Application {
public:

	Application3D();
	virtual ~Application3D();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

protected:

	glm::mat4	m_viewMatrix;
	glm::mat4	m_projectionMatrix;

	Scene* m_scene;

	aie::ShaderProgram	m_shader;
	aie::ShaderProgram	m_phong;
	aie::Texture		m_texture;
	Mesh				m_quad;
	glm::mat4			m_quadTransform;
	aie::OBJMesh		m_customMesh;
	glm::mat4			m_customMeshTransform;
	glm::vec3			m_ambient;

	Camera*				m_camera;

};