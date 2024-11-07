#pragma once
#include "../Math/Vector/Vector4.h"
#include "../Math/Vector/Vector2.h"
#include "../Math/Vector/Vector3.h"

struct SpriteVertex
{
	Vector4 position;
	Vector2 texcoord;
	Vector3 normal;
};