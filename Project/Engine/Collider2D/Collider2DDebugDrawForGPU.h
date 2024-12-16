#pragma once
#include "../Math/Matrix/Matrix4x4.h"

/// <summary>
/// 2D衝突オブジェクトデバッグ描画でGPUに送るデータ
/// </summary>
struct Collider2DDebugDrawForGPU
{

	Matrix4x4 WVP; // ワールドビュープロジェクション
	Matrix4x4 World; // ワールドトランスフォーム
	uint32_t textureNum; // テクスチャ番号

};
