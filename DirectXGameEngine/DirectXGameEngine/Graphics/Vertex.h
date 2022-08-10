#pragma once
#include <DirectXMath.h>

struct Vertex
{
	Vertex() {};
	Vertex(const float x, const float y, const float z, const float r, const float g, const float b)
		: pos_(x, y, z),
		color_(r, g, b){};

	DirectX::XMFLOAT3 pos_;
	DirectX::XMFLOAT3 color_;
};