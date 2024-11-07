#pragma once
#include "../../Math/Matrix/Matrix4x4.h"
#include "../Transform/TransformStructure.h"

/// <summary>
/// ノードデータ
/// </summary>
struct NodeData
{
	Matrix4x4 localMatrix; // ローカル行列
	uint32_t meshNum; // メッシュ番号
	std::string name; // 名前
	int32_t parentIndex; // 親行列
	Matrix4x4 matrix; //最終的なワールド行列
	Matrix4x4 offsetMatrix; // オフセット行列
	QuaternionTransform initTransform; // 初期トランスフォーム
	std::vector<uint32_t> childrenIndexes;

};
