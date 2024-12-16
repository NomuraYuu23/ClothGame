#include "../../Math/Vector/Vector3.h"
#include "../../Math/Vector/Vector4.h"

/// <summary>
/// 線描画時にGPUに送るデータ
/// </summary>
struct LineForGPU
{

	Vector3 position[2]; // 位置
	Vector4 color[2]; // 色

};
