#pragma once
#include "../Math/Vector/Vector3.h"

/// <summary>
/// カメラのGPUに送るデータ
/// </summary>
struct CameraForGPU{
	// ワールド座標
	Vector3 worldPosition;
};
