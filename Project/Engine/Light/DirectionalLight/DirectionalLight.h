#pragma once
#include <DirectXMath.h>
#include <Windows.h>
#include <d3d12.h>
#include <string>
#include <wrl.h>
#include <dxcapi.h>

#pragma comment(lib, "dxcompiler.lib")

#include "DirectionalLightData.h"

/// <summary>
/// 平行光源
/// </summary>
class DirectionalLight
{

public: // 静的メンバ関数

	/// <summary>
	/// 静的初期化
	/// </summary>
	/// <param name="device">デバイス</param>
	static void StaticInitialize(ID3D12Device* device);

	/// <summary>
	/// 光源生成
	/// </summary>
	/// <returns></returns>
	static DirectionalLight* Create();

private: // 静的メンバ変数

	// デバイス
	static ID3D12Device* sDevice_;
	// コマンドリスト
	static ID3D12GraphicsCommandList* sCommandList_;

public: // メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="directionalLightData">平行光源データ</param>
	void Update(const DirectionalLightData& directionalLightData);

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="cmdList">コマンドリスト</param>
	/// <param name="rootParameterIndex">ルートパラメータ番号</param>
	void Draw(ID3D12GraphicsCommandList* cmdList, uint32_t rootParameterIndex);

private: // メンバ変数

	// バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> directionalLightBuff_;

	// マップ
	DirectionalLightData* directionalLightMap_ = nullptr;

};

