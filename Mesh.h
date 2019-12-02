#pragma once

#pragma warning(disable : 4996)

#include <vector>

#include "SimpleMath.h"
#include "..\Content\ShaderStructures.h"
#include <Model.h>
#include <Effects.h>
#include <vector>

#include <fstream>
#include <iostream>

#include "tiny_obj_loader.h"

#include "DirectXTex.h"

using namespace DirectX::SimpleMath;
using namespace DirectX;
using namespace tinyobj;

namespace BS_Engine
{
	struct Mesh
	{
	private:
		void ResetMesh()
		{
			vertices.clear();
			uvs.clear();
			normals.clear();
			vIndices.clear();
			uvIndices.clear();
			nIndices.clear();
		}

	public:
		std::vector<Vector3> vertices, normals;
		std::vector<Vector2> uvs;
		std::vector<unsigned int> vIndices, uvIndices, nIndices;

		std::vector<VertexData> verticesData;

		std::unique_ptr<DirectX::Model> model;
		
		std::shared_ptr<ScratchImage> diffuseTex;

		Mesh()
		:vertices(), uvs(), normals(), vIndices(), uvIndices(), nIndices(), verticesData()
		{
		}
		/*
		bool LoadMesh(const std::string& filePath)
		{
			Assimp::Importer importer;

			const aiScene* pScene = importer.ReadFile(filePath,
				aiProcess_Triangulate | aiProcess_ConvertToLeftHanded);

			if (pScene == nullptr)
				return false;
		}
		*/
		
		void LoadCMO(const std::shared_ptr<DX::DeviceResources> deviceResources, const wchar_t* fileName)
		{
			auto device = deviceResources->GetD3DDevice();
			auto m_fx = std::make_unique<DGSLEffectFactory>(device);
			model = Model::CreateFromCMO(device, fileName, *m_fx);
		}

		bool Load(char path[])
		{
			std::string warn, err;
			attrib_t attrib;
			std::vector<shape_t> shapes;
			std::vector<material_t> materials;

			tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, path);
			for (int i = 0; i < attrib.vertices.size() / 3; i++)
			{
				Vector3 vertex = Vector3(attrib.vertices[3 * i], attrib.vertices[3 * i + 1], -attrib.vertices[3 * i + 2]);
				vertices.push_back(vertex);
			}

			for (int i = 0; i < attrib.texcoords.size() / 2; i++)
			{
				Vector2 uv = Vector2(attrib.texcoords[2 * i], attrib.texcoords[2 * i + 1]);
				uvs.push_back(uv);
			}
			
			for (int i = 0; i < shapes[0].mesh.indices.size(); i++)
			{
				vIndices.push_back(shapes[0].mesh.indices[i].vertex_index);
			}

			if (uvs.size() <= 0)
			{
				for (int i = 0; i < vertices.size(); i++)
				{
					verticesData.push_back(VertexData(vertices[i]));
				}
			}
			else
			{
				verticesData.resize(vertices.size());
				for (const auto &idx : shapes[0].mesh.indices)
				{
					verticesData[idx.vertex_index] = VertexData(vertices[idx.vertex_index], uvs[idx.texcoord_index]);
				}
			}
			return true;
		}
	};
}