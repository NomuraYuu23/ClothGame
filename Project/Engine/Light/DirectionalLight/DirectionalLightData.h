#pragma once
#include "../../Math/Vector/Vector3.h"
#include "../../Math/Vector/Vector4.h"

/// <summary>
/// 平行光源データ
/// </summary>
struct DirectionalLightData {
	Vector4 color; // 色
	Vector3 direction; // 方向
	float intencity; // 強度
};
