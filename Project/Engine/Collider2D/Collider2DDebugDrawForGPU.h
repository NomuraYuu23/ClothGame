#pragma once
#include "../Math/Matrix/Matrix4x4.h"

struct Collider2DDebugDrawForGPU
{

	Matrix4x4 WVP;
	Matrix4x4 World;
	uint32_t textureNum;

};
