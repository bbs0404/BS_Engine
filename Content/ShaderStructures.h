#pragma once

#include "SimpleMath.h"
using namespace DirectX::SimpleMath;

namespace BS_Engine
{
	// MVP 매트릭스를 꼭짓점 셰이더로 보내는 데 사용되는 상수 버퍼입니다.
	struct ModelViewProjectionConstantBuffer
	{
		DirectX::XMMATRIX model;
		DirectX::XMMATRIX view;
		DirectX::XMMATRIX projection;
	};

	// 꼭짓점별 데이터를 꼭짓점 셰이더로 보내는 데 사용됩니다.
	struct VertexPositionColor
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT3 color = DirectX::XMFLOAT3(1,1,1);

		VertexPositionColor()
			:pos(0,0,0)
		{
		}
		VertexPositionColor(DirectX::XMFLOAT3 pos)
			:pos(pos)
		{
		}
		VertexPositionColor(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 color)
			:pos(pos), color(color)
		{
		}
	};

	struct VertexData
	{
		Vector3 pos;
		Vector2 uv;
		Color color;

		VertexData()
			:pos(0, 0, 0), uv(0, 0), color(1, 1, 1, 1)
		{

		}

		VertexData(Vector3 pos)
			:pos(pos), uv(0, 0), color(1, 1, 1, 1)
		{

		}

		VertexData(Vector3 pos, Color color)
			:pos(pos), uv(0, 0), color(color)
		{

		}

		VertexData(Vector3 pos, Vector2 uv, Color color = Color(1,1,1,1))
			:pos(pos), uv(uv), color(color)
		{

		}
	};
}