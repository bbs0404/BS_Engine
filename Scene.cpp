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
		obj->transform.position = Vector3(0, 0, 0);

		obj = new GameObject();
		obj->transform.position = Vector3(5, 5, 5);

		Camera::main->gameObject.transform.LookAt(obj->transform);

		obj->transform.scale = Vector3(0.1f, 0.1f, 0.1f);
		gameObjects.push_back(obj);
		
		MeshRenderer* renderer = obj->AddComponent<MeshRenderer>();
		renderer->material = std::shared_ptr<Material>(new Material());
		renderer->material->LoadTex(deviceResources, L"4dollars.jpg");
		std::shared_ptr<Mesh> mesh = std::shared_ptr<Mesh>(new Mesh());
		mesh->Load("bs.txt");
		renderer->mesh = mesh;
		
		/*
		LineRenderer* lr = obj->AddComponent<LineRenderer>();
		std::vector<VertexPositionColor> vertices = std::vector<VertexPositionColor>();
		VertexPositionColor vertex;
		vertex.pos = Vector3(5, 5, 0);
		vertex.color = Vector3(1, 0, 0);
		vertices.push_back(vertex);
		vertex.pos = Vector3(5, -5, 0);
		vertex.color = Vector3(0, 1, 0);
		vertices.push_back(vertex);
		vertex.pos = Vector3(-5, -5, 0);
		vertex.color = Vector3(0, 0, 1);
		vertices.push_back(vertex);
		vertex.pos = Vector3(-5, 5, 0);
		vertex.color = Vector3(0, 0, 0);
		vertices.push_back(vertex);
		vertex.pos = Vector3(5, 5, 0);
		vertex.color = Vector3(1, 0, 0);
		vertices.push_back(vertex);
		lr->SetPositions(vertices);
		lr->material = std::shared_ptr<Material>(new Material());
		*/
	}
	
	Scene::~Scene()
	{
	}

	void Scene::Update(DX::StepTimer const& timer)
	{
		gameObjects[1]->transform.Rotate(Vector3(0, timer.GetElapsedSeconds() * 100, 0));
		//Camera::main->gameObject.transform.Rotate(Vector3(0, timer.GetElapsedSeconds() * 100, 0));
		for each(GameObject* gameObject in gameObjects)
		{
			gameObject->Update(timer);
		}
	}
}