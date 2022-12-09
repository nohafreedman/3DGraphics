#include "Scene.h"
#include "MeshInstance.h"
#include "Gizmos.h"
#include "imgui.h"

Scene::Scene(Camera* camera, glm::vec2 windowSize)
{
	m_camera = camera;
	m_windowSize = windowSize;
	m_sun.direction = vec3(1, 1, 1);
	m_sun.colour = vec3(1, 0, 1);
	m_ambient = vec3(.25f, .25f, .25f);
	m_numLights = 0;
	
}

Scene::~Scene()
{
	for (auto it = m_meshes.begin(); it != m_meshes.end(); it++)
		delete* it;
}

void Scene::AddMesh(glm::mat4 transform, aie::OBJMesh* mesh, aie::ShaderProgram* shader)
{
	MeshInstance* newMesh = new MeshInstance(mesh, shader);
	newMesh->setTransform(transform);
	m_meshes.push_back(newMesh);
}

void Scene::AddLight(Light light)
{
	m_numLights++;
	m_pointLights.push_back(light);
}

void Scene::AddLight(glm::vec3 _pos, glm::vec3 _col, float _intensity)
{
	m_numLights++;
	m_pointLights.push_back(Light(_pos, _col, _intensity));
}

/// <summary>
/// Added an update to the scene class so that I can put ImGUI stuff in here and not clog the application class
/// </summary>
/// <param name="deltaTime"></param>
void Scene::update(float deltaTime)
{
	ImGui::Begin("Debugging");
	ImGui::Text("Controls: WASD, hold right click to rotate cam");
	if (ImGui::Checkbox("Show Light Gizmo", &debugLights))
		aie::Gizmos::clear();
	switch (mapDebugger) {
	default:
		ImGui::Text("Default Lighting");
		break;
	case 0:
		ImGui::Text("Default Lighting");
		break;
	case 1:
		ImGui::Text("Showing Normal Maps");
		break;
	case 2:
		ImGui::Text("Showing Diffuse Maps");
		break;
	case 3:
		ImGui::Text("Showing Specular Maps");
		break;
	case 4:
		ImGui::Text("Showing Ambient Maps");
		break;
	}
	ImGui::DragInt("Debug Texture Maps", &mapDebugger, 0.1f, 0, 4);
	ImGui::DragInt("Current Light", &lightIterator, 1, 0, m_numLights);
	if (ImGui::Button("<", ImVec2(0, 0))) {
		if (lightIterator > 0)
			lightIterator--;
	}
	if (ImGui::Button(">", ImVec2(0, 0))) {
		if (lightIterator < m_numLights - 1)
			lightIterator++;
	}
	if(ImGui::DragFloat3("Position", &m_pointLights[lightIterator].position[0], 0.1f))
		aie::Gizmos::clear();
	if (ImGui::DragFloat3("Colour", &m_pointLights[lightIterator].colour[0], 0.1f))
		aie::Gizmos::clear();
	ImGui::End();

}

void Scene::draw()
{

	m_projectionMatrix = m_camera->getProjectionMatrix(m_windowSize.x, m_windowSize.y);
	m_viewMatrix = m_camera->getViewMatrix();

	auto pvm = m_projectionMatrix * m_viewMatrix;

	// iterates through lights and puts their information into arrays to be sent to the GPU
	// also if we're debugging, shows the position of the point lights with a sphere gizmo
	for (int i = 0; i < 4 && i < m_pointLights.size(); i++) {
		m_pointLightPositions[i] = m_pointLights[i].position;
		m_pointLightColours[i] = m_pointLights[i].colour;

		if (debugLights) {
			vec4 colour = vec4(m_pointLightColours[i].x, m_pointLightColours[i].y, m_pointLightColours[i].z, 1);
			aie::Gizmos::addSphere(m_pointLightPositions[i], 0.2f, 5, 5, colour);
		}
	}

	// draws the mesh instances and sends them which texture map we want to debug
	for (auto i = m_meshes.begin(); i != m_meshes.end(); i++) {
		MeshInstance* instance = *i;
		instance->setDebugInt(mapDebugger);
		instance->draw(this, pvm);
	}

}
