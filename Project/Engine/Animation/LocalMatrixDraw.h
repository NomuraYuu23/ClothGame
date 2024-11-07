#pragma once
#include "LocalMatrixManager.h"
#include "../3D/Transform/WorldTransform.h"
#include "../3D/Line/DrawLine.h"

class LocalMatrixDraw
{

public: // 関数

	/// <summary>
	/// 描画用マップ
	/// </summary>
	/// <param name="localMatrixManager">ローカル行列マネージャー</param>
	/// <param name="worldTransform">ワールドトランスフォーム</param>
	/// <param name="drawLine">線描画ポインタ</param>
	static void DrawMap(
		LocalMatrixManager* localMatrixManager,
		WorldTransform* worldTransform,
		DrawLine* drawLine);

};

