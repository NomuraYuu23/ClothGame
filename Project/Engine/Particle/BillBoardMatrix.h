#pragma once
#include "../Math/Matrix/Matrix4x4.h"
#include "../Camera/BaseCamera.h"

/// <summary>
/// ビルボード行列
/// </summary>
class BillBoardMatrix
{

public: // 関数

	/// <summary>
	/// 全軸ビルボード
	/// </summary>
	/// <param name="camera">カメラ</param>
	/// <returns></returns>
	static Matrix4x4 GetBillBoardMatrixAll(BaseCamera& camera);

	/// <summary>
	/// X軸ビルボード
	/// </summary>
	/// <param name="camera">カメラ</param>
	/// <returns></returns>
	static Matrix4x4 GetBillBoardMatrixX(BaseCamera& camera);

	/// <summary>
	/// Y軸ビルボード
	/// </summary>
	/// <param name="camera">カメラ</param>
	/// <returns></returns>
	static Matrix4x4 GetBillBoardMatrixY(BaseCamera& camera);

	/// <summary>
	/// Z軸ビルボード
	/// </summary>
	/// <param name="camera">カメラ</param>
	/// <returns></returns>
	static Matrix4x4 GetBillBoardMatrixZ(BaseCamera& camera);

};

