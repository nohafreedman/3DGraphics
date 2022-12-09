#pragma once

#include "Shader.h"
#include "OBJMesh.h"
#include "Camera.h"
#include <glm/mat4x4.hpp>

class MeshInstance;

struct Directional {
	glm::vec3 direction;
	glm::vec3 colour;
};

struct Light {
	glm::vec3 position;
	glm::vec3 colour;
	Light(glm::vec3 _pos, glm::vec3 _col, float _intensity) {
		position = _pos;
		colour = _col * _intensity;
	}
};

/// <summary>
/// I tried to do all of this functionality just in the Application, but it got really messy with a bunch of circular dependencies so
/// splitting it into a Scene was super handy even though its a bit cumbersome
/// </summary>
class Scene
{
public:
	Scene(Camera* camera, glm::vec2 windowSize);
	~Scene();
	void AddMesh(glm::mat4 transform, aie::OBJMesh* mesh, aie::ShaderProgram* shader);
	void AddLight(Light light);
	void AddLight(glm::vec3 _pos, glm::vec3 _col, float _intensity);
	void update(float deltaTime);
	void draw();

	float getNumLights() { return m_numLights; }

	glm::vec3 getSunDirection() { return m_sun.direction; }
	glm::vec3 getSunColour() { return m_sun.colour; }
	glm::vec3 getAmbientColour() { return m_ambient; }
	glm::vec3* getPointPositions() { return &m_pointLightPositions[0]; }
	glm::vec3* getPointColours() { return &m_pointLightColours[0]; }
	void setDebug(bool debug) { debugLights = debug; }

	glm::mat4 getViewMatrix() { return m_viewMatrix; }

	std::vector<MeshInstance*>& getMeshInstances() { return m_meshes; }
	std::vector<Light>& getPointLights() { return m_pointLights; }

protected:
	Camera* m_camera;
	glm::vec2 m_windowSize;
	Directional m_sun;
	glm::vec3 m_ambient;
	std::vector<MeshInstance*> m_meshes;
	// shows a gizmo of where the light sources are
	bool debugLights = true;
	// used to switch between lights
	int lightIterator = 0;
	// debugs texture maps
	int mapDebugger = 0;

	glm::mat4	m_viewMatrix;
	glm::mat4	m_projectionMatrix;

	std::vector<Light>	m_pointLights;

	glm::vec3			m_pointLightPositions[4];
	glm::vec3			m_pointLightColours[4];

	int					m_numLights;

};

