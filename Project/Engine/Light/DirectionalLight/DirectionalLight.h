#pragma once
#include <DirectXMath.h>
#include <Windows.h>
#include <d3d12.h>
#include <string>
#include <wrl.h>
#include <dxcapi.h>

#pragma comment(lib, "dxcompiler.lib")

#include "DirectionalLightData.h"

class DirectionalLight
{

public:

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

private:

	// デバイス
	static ID3D12Device* sDevice;
	// コマンドリスト
	static ID3D12GraphicsCommandList* sCommandList;

public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update(const DirectionalLightData& directionalLightData);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ID3D12GraphicsCommandList* cmdList, uint32_t rootParameterIndex);

private:

	Microsoft::WRL::ComPtr<ID3D12Resource> directionalLightBuff_;

	DirectionalLightData* directionalLightMap = nullptr;

};

