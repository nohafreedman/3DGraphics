#include "MeshInstance.h"

MeshInstance::MeshInstance()
{

}

/// <summary>
/// Creates a new mesh instance with a pointer to the mesh data and a pointer to the shader we're using.
/// I think this is proper instancing, seeing as it stores the mesh once in memory and just accesses its memory, but im not sure.
/// </summary>
/// <param name="_mesh"></param>
/// <param name="_shader"></param>
MeshInstance::MeshInstance(aie::OBJMesh* _mesh, aie::ShaderProgram* _shader)
{
	mesh = _mesh;
	shader = _shader;
	debugInt = new int(0);
	transform = {
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};
}

/// <summary>
/// Can't be bothered going back to my math library to remember how matrix rotations work so im using this mess from the tutorial.
/// </summary>
/// <param name="position">self explainatory</param>
/// <param name="eulerAngles">rotation in xyz</param>
/// <param name="scale">self explainatory</param>
void MeshInstance::moveTransform(glm::vec3 position, glm::vec3 eulerAngles, glm::vec3 scale)
{
	transform = glm::translate(glm::mat4(1), position)
		* glm::rotate(glm::mat4(1), glm::radians(eulerAngles.z), glm::vec3(0, 0, 1))
		* glm::rotate(glm::mat4(1), glm::radians(eulerAngles.y), glm::vec3(0, 1, 0))
		* glm::rotate(glm::mat4(1), glm::radians(eulerAngles.x), glm::vec3(1, 0, 0))
		* glm::scale(glm::mat4(1), scale);
}

void MeshInstance::update(float deltaTime)
{

}

/// <summary>
/// Draw function. The tutorial had EACH mesh instance calculating half of the PVM transform that they didn't need to, so I neatened it up
/// by giving a reference to just the concatenated projection and view matrixes
/// </summary>
/// <param name="scene"></param>
/// <param name="pv"></param>
void MeshInstance::draw(Scene* scene, glm::mat4 pv)
{

	shader->bind();

	// bind sun information
	shader->bindUniform("LightDirection", scene->getSunDirection());
	shader->bindUniform("LightColour", scene->getSunColour());
	shader->bindUniform("AmbientColour", scene->getAmbientColour());
	shader->bindUniform("debug", *debugInt);

	// bind individual point lights
	int numLights = scene->getNumLights();
	shader->bindUniform("numLights", numLights);
	shader->bindUniform("PointLightPosition", numLights, scene->getPointPositions());
	shader->bindUniform("PointLightColour", numLights, scene->getPointColours());

	// much quicker than what the tutorial had
	glm::mat4 pvm = pv * transform;

	shader->bindUniform("ProjectionViewModel", pvm);

	shader->bindUniform("ModelMatrix", transform);

	shader->bindUniform("cameraPosition", vec3(glm::inverse(scene->getViewMatrix())[3]));

	mesh->draw();
}
