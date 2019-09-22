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

using namespace DirectX::SimpleMath;
using namespace DirectX;
using namespace tinyobj;

namespace BS_Engine
{
	struct VertexData
	{
		Vector3 vertex;
		Vector2 uv;
		Vector3 normal;

		VertexData(Vector3 v, Vector2 uv, Vector3 n)
		{
			vertex = v;
			this->uv = uv;
			normal = n;
		}
	};

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

		std::vector<VertexPositionColor> vpcs;
		std::unique_ptr<DirectX::Model> model;

		Mesh()
		:vertices(), uvs(), normals(), vIndices(), uvIndices(), nIndices(), vpcs()
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
				Vector3 vertex = Vector3(attrib.vertices[3 * i], attrib.vertices[3 * i + 1], attrib.vertices[3 * i + 2]);
				vertices.push_back(vertex);
			}
			for (int i = 0; i < shapes[0].mesh.indices.size(); i++)
			{
				vIndices.push_back(shapes[0].mesh.indices[i].vertex_index);
			}

			/*

			std::ifstream fin;
			char input;

			fin.open(path);
			if (fin.fail())
				return false;

			fin.get(input);
			while (!fin.eof())
			{
				if (input == 'v')
				{
					fin.get(input);
					if (input == ' ')
					{
						Vector3 vertex;
						fin >> vertex.x >> vertex.y >> vertex.z;
						vertex.z = vertex.z * -1.0f;
						vertices.push_back(vertex);
					}

					if (input == 't')
					{
						Vector2 uv;
						fin >> uv.x >> uv.y;
						uv.y = 1.0f - uv.y;
						uvs.push_back(uv);
					}

					if (input == 'n')
					{
						Vector3 normal;
						fin >> normal.x >> normal.y >> normal.z;
						normal.z = normal.z * -1.0f;
						normals.push_back(normal);
					}
				}

				if (input == 'f')
				{
					fin.get(input);
					if (input == ' ')
					{
						char input2;
						unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
						fin >> vertexIndex[0] >> input2 >> uvIndex[0] >> input2 >> normalIndex[0];
						fin >> vertexIndex[1] >> input2 >> uvIndex[1] >> input2 >> normalIndex[1];
						fin >> vertexIndex[2] >> input2 >> uvIndex[2] >> input2 >> normalIndex[2];
						
						vIndices.push_back(vertexIndex[2] - 1);
						vIndices.push_back(vertexIndex[1] - 1);
						vIndices.push_back(vertexIndex[0] - 1);
						uvIndices.push_back(uvIndex[2] - 1);
						uvIndices.push_back(uvIndex[1] - 1);
						uvIndices.push_back(uvIndex[0] - 1);
						nIndices.push_back(normalIndex[2] - 1);
						nIndices.push_back(normalIndex[1] - 1);
						nIndices.push_back(normalIndex[0] - 1);
					}
				}

				while (fin.eof() || input != '\n')
				{
					fin.get(input);
				}

				fin.get(input);
			}
			fin.close();
			*/
			for each (auto vertex in vertices)
			{
				vpcs.push_back(VertexPositionColor(vertex));
			}
			return true;
		}

		///<summary>Try to load .obj file. If failed, return false and reset all mesh data.</summary>
		bool LoadObj(char path[])
		{
			FILE * file = fopen(path, "r");
			ResetMesh();
			if (file == NULL)
			{
				return false;
			}

			while (1)
			{
				char header[128];
				int res = fscanf(file, "%s", header);
				if (res == EOF)
					break;
				if (strcmp(header, "v") == 0)
				{
					Vector3 vertex;
					fscanf(file, "%f %f %f\n", &vertex.x, &vertex.z, &vertex.y);
					vertices.push_back(vertex);
				}
				else if (strcmp(header, "vt") == 0)
				{
					Vector2 uv;
					fscanf(file, "%f %f\n", &uv.x, &uv.y);
					uvs.push_back(uv);
				}
				else if (strcmp(header, "vn") == 0)
				{
					Vector3 normal;
					fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
					normals.push_back(normal);
				}
				else if (strcmp(header, "f") == 0)
				{
					unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
					int matches = fscanf(file, "%d//%d %d//%d %d//%dn",
						&vertexIndex[0], /*&uvIndex[0],*/ &normalIndex[0],
						&vertexIndex[1], /*&uvIndex[1],*/ &normalIndex[1],
						&vertexIndex[2], /*&uvIndex[2],*/ &normalIndex[2]
					);

					if (matches != 6)												//Wrong format
					{
						ResetMesh();
						return false;
					}
					
					vIndices.push_back(vertexIndex[2]);
					vIndices.push_back(vertexIndex[1]);
					vIndices.push_back(vertexIndex[0]);
					/*
					uvIndices.push_back(uvIndex[2]);
					uvIndices.push_back(uvIndex[1]);
					uvIndices.push_back(uvIndex[0]);
					*/
					nIndices.push_back(normalIndex[2]);
					nIndices.push_back(normalIndex[1]);
					nIndices.push_back(normalIndex[0]);
					
				}
			}
			for each (auto vertex in vertices)
			{
				vpcs.push_back(VertexPositionColor(vertex));
			}

			return true;
		}
	};
}