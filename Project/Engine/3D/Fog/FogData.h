#pragma once
#include "../../Math/Vector/Vector4.h"

/// <summary>
/// 霧データ
/// </summary>
struct FogData
{

	Vector4 color; // 色
	float fagNear; // 開始位置
	float fagFar; // 終了位置

};
