#pragma once
#include "../../Math/Vector/Vector3.h"
#include "../../Math/DeltaTime.h"

class Gravity
{

public: // 関数

	/// <summary>
	/// 重力処理実行
	/// </summary>
	/// <returns></returns>
	static Vector3 Execute();

private: // 変数

	// 向き
	static Vector3 dirction_;

	// 力
	static float power_;

public: // アクセッサ
	
	// 向き
	static void SetDirction(const Vector3& dirction) { dirction_ = dirction; }

	// 力
	static void SetPower(const float& power) { power_ = power;	}

	// 力
	static float GetPower() { return power_; }

};

