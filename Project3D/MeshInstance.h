#pragma once

#include "Shader.h"
#include "Mesh.h"
#include "OBJMesh.h"
#include "Camera.h"
#include "Scene.h"
#include <glm/mat4x4.hpp>
#include <glm/ext.hpp>

/// <summary>
/// Instances meshes so that I dont have to do it all manually in the Application
/// </summary>
class MeshInstance
{
public:

	int* debugInt;

	MeshInstance();
	MeshInstance(aie::OBJMesh* _mesh, aie::ShaderProgram* _shader);

	void moveTransform(glm::vec3 position, glm::vec3 eulerAngles, glm::vec3 scale);
	void setTransform(glm::mat4 _transform) { transform = _transform; }
	void setDebugInt(int debug) { *debugInt = debug; }
	glm::mat4 getTransform() { return transform; }
	void update(float deltaTime);
	void draw(Scene* scene, glm::mat4 pv);

protected:
	glm::mat4 transform;
	aie::OBJMesh* mesh;
	aie::ShaderProgram* shader;

};

