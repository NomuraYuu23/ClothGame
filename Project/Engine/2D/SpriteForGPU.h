#pragma once
#include "../Math/Matrix/Matrix4x4.h"

/// <summary>
/// スプライト描画で使う構造体
/// </summary>
struct SpriteForGPU {

	Matrix4x4 WVP; // ワールドビュープロジェクション
	Matrix4x4 World; // ワールドトランスフォーム

};
