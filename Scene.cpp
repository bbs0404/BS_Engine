#include "pch.h"
#include "Scene.h"
#include "Common\DirectXHelper.h"
#include "..\Common\DeviceResources.h"
#include "..\Content\ShaderStructures.h"
#include "..\Common\StepTimer.h"

#include "SceneRenderer.h"

namespace BS_Engine
{
	Scene::Scene(const std::shared_ptr<DX::DeviceResources> deviceResources)
		:m_deviceResources(deviceResources)
	{
		if (SceneRenderer::activeScene == nullptr)
			SceneRenderer::activeScene = this;

		GameObject* obj = new GameObject();
		gameObjects.push_back(obj);
		obj->AddComponent<Camera>()->SetAsMain();
		obj->transform.position = Vector3(0, 0, 10);

		obj = new GameObject();
		obj->transform.position = Vector3(0, -1, 0);
		gameObjects.push_back(obj);
		MeshRenderer* renderer = obj->AddComponent<MeshRenderer>();
		renderer->material = std::shared_ptr<Material>(new Material());
		std::shared_ptr<Mesh> mesh = std::shared_ptr<Mesh>(new Mesh());
		mesh->Load("teapot.txt");
		renderer->mesh = mesh;

		/*
		LineRenderer* renderer = obj->AddComponent<LineRenderer>();
		std::vector<VertexPositionColor> vertices = std::vector<VertexPositionColor>();
		VertexPositionColor vertex;
		vertex.pos = Vector3(2, 2, 0);
		vertex.color = Vector3(1, 0, 0);
		vertices.push_back(vertex);
		vertex.pos = Vector3(2, -2, 0);
		vertex.color = Vector3(0, 1, 0);
		vertices.push_back(vertex);
		vertex.pos = Vector3(-2, -2, 0);
		vertex.color = Vector3(0, 0, 1);
		vertices.push_back(vertex);
		vertex.pos = Vector3(-2, 2, 0);
		vertex.color = Vector3(0, 0, 0);
		vertices.push_back(vertex);
		vertex.pos = Vector3(2, 2, 0);
		vertex.color = Vector3(1, 0, 0);
		vertices.push_back(vertex);
		renderer->SetPositions(vertices);
		renderer->material = std::shared_ptr<Material>(new Material());
		*/
	}
	
	Scene::~Scene()
	{
	}

	void Scene::Update(DX::StepTimer const& timer)
	{
		gameObjects[1]->transform.rotation += Vector3(timer.GetElapsedSeconds() * 100, timer.GetElapsedSeconds() * 100, 0);
		//Camera::main->gameObject.transform.rotation += Vector3(0, timer.GetElapsedSeconds() * 100, 0);
		for each(GameObject* gameObject in gameObjects)
		{
			gameObject->Update(timer);
		}
	}
}