#pragma once
#include "../Collider/OBB/OBB.h"

/// <summary>
/// 押し出し処理
/// </summary>
class Extrusion
{

public: // メンバ関数

	/// <summary>
	/// OBBとOBBの押し出し
	/// </summary>
	/// <param name="pushedOut">押し出される</param>
	/// <param name="pushOut">押し出す</param>
	/// <returns>押し出し距離</returns>
	static Vector3 OBBAndOBB(OBB* pushedOut, OBB* pushOut);

};

