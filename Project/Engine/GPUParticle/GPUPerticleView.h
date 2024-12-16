#pragma once
#include "../Math/Matrix/Matrix4x4.h"

/// <summary>
/// パーティクルビューGPU版
/// </summary>
struct GPUParticleView
{

	Matrix4x4 viewProjection; // ビュープロジェクション
	Matrix4x4 billboardMatrix; //ビルボード行列

};
