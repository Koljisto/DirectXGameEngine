#pragma once
#include <DirectXMath.h>

struct Vertex
{
	Vertex() {};
	Vertex(const float x, const float y)
		: pos_(x, y) {};

	DirectX::XMFLOAT2 pos_;
};