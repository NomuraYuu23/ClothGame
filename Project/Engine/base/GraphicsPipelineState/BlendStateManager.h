#pragma once
#include <d3d12.h>
#include <string>
#include <dxcapi.h>
#include <cassert>
#include <format>
#include <wrl.h>
#include <array>
#include <vector>

/// <summary>
/// ブレンドの名前
/// </summary>
enum BlendStateIndex {
	kBlendStateIndexNormal,
	kBlendStateIndexAdd,
	kBlendStateIndexOfCount,
};

class BlendStateManager
{

public: // 関数
	
	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns></returns>
	static BlendStateManager* GetInstance();

	/// <summary>
	/// 初期化
	/// </summary>
	static void Initialize();

public: // 変数

	// ブレンドステート
	static std::array<D3D12_BLEND_DESC, kBlendStateIndexOfCount> blendStates_;

private: // シングルトン

	BlendStateManager() = default;
	~BlendStateManager() = default;
	BlendStateManager(const BlendStateManager&) = delete;
	BlendStateManager& operator=(const BlendStateManager&) = delete;

};

