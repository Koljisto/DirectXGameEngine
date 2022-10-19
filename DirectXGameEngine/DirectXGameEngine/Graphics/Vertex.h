#pragma once
#include <DirectXMath.h>

struct Vertex
{
	Vertex() {};
	Vertex(const float x, const float y, const float z, const float u, const float v)
		: pos_(x, y, z),
		texture_coord_(u, v){};

	DirectX::XMFLOAT3 pos_;
	DirectX::XMFLOAT2 texture_coord_;
};