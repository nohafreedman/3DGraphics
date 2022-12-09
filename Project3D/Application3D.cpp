#include "Application3D.h"
#include "Gizmos.h"
#include "Input.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <imgui.h>
#include "MeshInstance.h"

using glm::vec3;
using glm::vec4;
using glm::mat4;
using aie::Gizmos;

Application3D::Application3D() {

}

Application3D::~Application3D() {

}

bool Application3D::startup() {
	
	setBackgroundColour(0.25f, 0.25f, 0.25f);

	// initialise gizmo primitive counts
	Gizmos::create(10000, 10000, 10000, 10000);

	m_camera = new Camera(glm::vec3(-10, 2, 0));

	m_viewMatrix = m_camera->getViewMatrix();
	m_projectionMatrix = m_camera->getProjectionMatrix(getWindowWidth(), getWindowHeight());

	//m_shader.loadShader(aie::eShaderStage::VERTEX, "./shaders/simple.vert");
	//m_shader.loadShader(aie::eShaderStage::FRAGMENT, "./shaders/simple.frag");

	m_phong.loadShader(aie::eShaderStage::VERTEX, "./bin/shaders/phong.vert");
	m_phong.loadShader(aie::eShaderStage::FRAGMENT, "./bin/shaders/phong.frag");

	if (m_phong.link() == false) {
		printf("Shader Error! %s\n", m_phong.getLastError());
		return false;
	}

	if (m_customMesh.load("./bin/models/soulspear/soulspear.obj", true, true) == false) {
		printf("Mesh Loading Error!\n");
		return false;
	}

	m_customMeshTransform = {
		0.2f, 0, 0, 0,
		0, 0.2f, 0, 0,
		0, 0, 0.2f, 0,
		0, 0, 0, 0.2f
	};

	m_ambient = vec3(0, 0, 1);

	vec3 startPos = vec3(0, 0, 0);
	float roffset = 25;
	float xoffset = 2;

	m_scene = new Scene(m_camera, glm::vec2(getWindowWidth(), getWindowHeight()));
	m_scene->AddMesh(m_customMeshTransform, &m_customMesh, &m_phong);

	float firstPos = 0;
	float firstAng = .2f;

	// creates a big wheel of soulspears, i thought it looked cool
	for (int i = 0; i < 160; i++) {
		MeshInstance newMesh;
		vec3 pos(firstPos, 0, 0);
		vec3 rot(0, 0, firstAng);
		vec3 scl(1, 1, 1);
		firstPos += firstPos;
		firstAng += 3;
		newMesh.moveTransform(pos, rot, scl);
		mat4 newTransform = newMesh.getTransform();
		m_scene->AddMesh(newTransform, &m_customMesh, &m_phong);
	}

	// im honestly impressed at my own laziness
	// instead of just putting a function to rotate transforms in here
	// im creating a mesh, using its function to rotate a transform,
	// and then getting the rotated transform and ditching the mesh
	// really bad in every way but it took 5 seconds to put in so :/
	MeshInstance transformMaker;
	vec3 pos(0, 0, 0);
	vec3 rot(90, 0, 90);
	vec3 scl(1, 1, 1);
	transformMaker.moveTransform(pos, rot, scl);
	mat4 pieceTransform = transformMaker.getTransform();
	m_scene->AddMesh(pieceTransform, &m_customMesh, &m_phong);

	vec3 posi(-5, 3, 1);
	vec3 colour(0, 1, 0);
	float intensity = 20;
	m_scene->AddLight(posi, colour, intensity);
	posi = vec3(5, 3, 1);
	colour = vec3(0, 0, 1);
	m_scene->AddLight(posi, colour, intensity);
	posi = vec3(2, 5, 1);
	m_scene->AddLight(posi, colour, intensity);
	

	return true;
}

void Application3D::shutdown() {

	Gizmos::destroy();
	delete m_scene;
}

void Application3D::update(float deltaTime) {

	// query time since application started
	float time = getTime();

	m_camera->update(deltaTime);

	// uncomment to debug cam pos
	/*ImGui::Begin("CAMERA!!!");
	ImGui::InputFloat3("Camera Position", &m_camera->m_position[0]);
	ImGui::End();*/

	m_scene->update(deltaTime);

	// quit if we press escape
	aie::Input* input = aie::Input::getInstance();

	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
}

void Application3D::draw() {

	// wipe the screen to the background colour
	clearScreen();

	// update perspective in case window resized
	m_projectionMatrix = m_camera->getProjectionMatrix(getWindowWidth(), getWindowHeight());
	m_viewMatrix = m_camera->getViewMatrix();

	auto pvm = m_projectionMatrix * m_viewMatrix;

	m_scene->draw();

	// draw 3D gizmos
	Gizmos::draw(m_projectionMatrix * m_viewMatrix);

	// draw 2D gizmos using an orthogonal projection matrix (or screen dimensions)
	Gizmos::draw2D((float)getWindowWidth(), (float)getWindowHeight());
}