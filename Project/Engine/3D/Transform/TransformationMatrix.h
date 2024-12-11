#pragma once
#include "../../Math/Matrix/Matrix4x4.h"

/// <summary>
/// トランスフォーム行列
/// </summary>
struct TransformationMatrix {
	Matrix4x4 WVP; // ワールドビュープロジェクション
	Matrix4x4 World; //ワールドトランスフォーム
	Matrix4x4 WorldInverseTranspose; //逆ワールドトランスフォーム
};
