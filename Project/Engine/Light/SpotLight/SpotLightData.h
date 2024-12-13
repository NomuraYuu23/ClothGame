#pragma once
#include "../../Math/Vector/Vector4.h"
#include "../../Math/Vector/Vector3.h"

/// <summary>
/// スポットライトデータ
/// </summary>
struct SpotLightData {
	Vector4 color; // ライト色
	Vector3 position; // ライトの位置
	float intencity; // 輝度
	Vector3 direction; // ライトの方向
	float distance; // ライトの届く距離
	float decay; // 減衰率
	float cosAngle; // スポットライトの余弦
	float cosFalloffStart; // フォールオフ開始位置
	bool used; // 使用している
	bool padding;
};
