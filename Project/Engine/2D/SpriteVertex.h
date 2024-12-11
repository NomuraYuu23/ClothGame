#pragma once
#include "../Math/Vector/Vector4.h"
#include "../Math/Vector/Vector2.h"
#include "../Math/Vector/Vector3.h"

/// <summary>
/// スプライト用頂点データ
/// </summary>
struct SpriteVertex
{
	Vector4 position; // 位置
	Vector2 texcoord; // テクスチャ座標
	Vector3 normal; // 法線
};