#pragma once
#include "../../Math/Vector/Vector4.h"
#include "../../Math/Vector/Vector3.h"

/// <summary>
/// ポイントライトデータ
/// </summary>
struct PointLightData{
	Vector4 color; // ライト色
	Vector3 position; // ライトの位置
	float intencity; // 輝度
	float radius; // ライトの届く最大距離
	float decay; // 減衰率
	bool used; // 使用している
	bool padding;
};
