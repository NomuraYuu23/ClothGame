#pragma once
#include <DirectXMath.h>
#include <Windows.h>
#include <d3d12.h>
#include <string>
#include <wrl.h>
#include <dxcapi.h>

#pragma comment(lib, "dxcompiler.lib")

#include "OutlineData.h"

/// <summary>
/// アウトライン
/// </summary>
class Outline
{

public: // 関数

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// マップ
	/// </summary>
	/// <param name="outlineData">アウトラインデータ</param>
	void Map(const OutlineData& outlineData);

public: // アクセッサ

	/// <summary>
	/// バッファ取得
	/// </summary>
	/// <returns></returns>
	ID3D12Resource* GetOutlineDataBuff() { return outlineDataBuff_.Get(); }

private: // 変数

	// バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> outlineDataBuff_;

	// 書き込むデータ
	OutlineData* outlineDataMap_{};

};

