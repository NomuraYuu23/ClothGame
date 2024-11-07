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
/// インプットレイアウトの名前
/// </summary>
enum InputLayoutIndex {
	kInputLayoutIndexNormal,
	kInputLayoutIndexNone,
	kInputLayoutIndexPositon,
	kInputLayoutIndexGPUParticle,
	kInputLayoutIndexOfCount,
};

class InputLayoutManager
{

public: // 関数

	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns></returns>
	static InputLayoutManager* GetInstance();

	/// <summary>
	/// 初期化
	/// </summary>
	static void Initialize();

public: // 変数

	// インプットレイアウト
	static std::array<D3D12_INPUT_LAYOUT_DESC, kInputLayoutIndexOfCount> inputLayouts_;
	// インプットレイアウトの要素
	static std::array< std::vector<D3D12_INPUT_ELEMENT_DESC>, kInputLayoutIndexOfCount> inputElementDescs_;

private: // シングルトン

	InputLayoutManager() = default;
	~InputLayoutManager() = default;
	InputLayoutManager(const InputLayoutManager&) = delete;
	InputLayoutManager& operator=(const InputLayoutManager&) = delete;

};

