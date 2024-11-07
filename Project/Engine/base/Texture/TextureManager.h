#pragma once

#include <array>
#include "../../../externals/DirectXTex/d3dx12.h"
#include <string>
#include <unordered_map>
#include <wrl.h>
#include "../../../externals/DirectXTex/DirectXTex.h"

#include "../WinApp.h"
#include "../DxCommon/DirectXCommon.h"
#include "../DescriptorHerpManager/SRVDescriptorHerpManager.h"

using namespace DirectX;

//テクスチャマネージャー
class TextureManager
{
public:

	struct Texture{
		// テクスチャリソース
		Microsoft::WRL::ComPtr<ID3D12Resource> resource;
		// シェーダーリソースビューのハンドル(CPU)
		CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV;
		// シェーダーリソースビューのハンドル(GPU)
		CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV;
		//  SRVのインデックス
		uint32_t indexDescriptorHeap;
		//名前
		std::string name;
		// 使っているか
		bool used;
	};

	/// <summary>
	/// 読み込み
	/// </summary>
	/// <param name="fileName">ファイル名</param>
	/// <returns>テクスチャハンドル</returns>
	static uint32_t Load(const std::string& fileName, DirectXCommon* dxCommon);

	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns></returns>
	static TextureManager* GetInstance();

	/// <summary>
	/// システム初期化
	/// </summary>
	/// <param name="device">デバイス</param>
	/// <param name="directoryPath"></param>
	void Initialize(ID3D12Device* device, std::string directoryPath = "Resources/");

	/// <summary>
	/// 全テクスチャリセット
	/// </summary>
	void ResetAll();

	/// <summary>
	/// テクスチャのリセット
	/// </summary>
	void ResetTexture();

	
	/// <summary>
	/// テクスチャのリセット
	/// </summary>
	void ResetTexture(const std::vector<uint32_t>& handles);

	/// <summary>
	/// リソース情報取得
	/// </summary>
	/// <param name="textureHandle">テクスチャハンドル</param>
	/// <returns>リソース情報</returns>
	const D3D12_RESOURCE_DESC GetResourceDesc(uint32_t textureHandle);

	/// <summary>
	/// ディスクリプタテーブルをセット
	/// </summary>
	/// <param name="commandList">コマンドリスト</param>
	/// <param name="rootParamIndex">ルートパラメータ番号</param>
	/// <param name="textureHandle">テクスチャハンドル</param>
	void SetGraphicsRootDescriptorTable(ID3D12GraphicsCommandList* commandList, UINT rootParamIndex, uint32_t textureHandle);

	/// <summary>
	/// ディスクリプタテーブルをセット
	/// </summary>
	/// <param name="commandList">コマンドリスト</param>
	/// <param name="rootParamIndex">ルートパラメータ番号</param>
	/// <param name="textureHandle">テクスチャハンドル</param>
	void SetComputeRootDescriptorTable(ID3D12GraphicsCommandList* commandList, UINT rootParamIndex, uint32_t textureHandle);

private:
	TextureManager() = default;
	~TextureManager() = default;
	TextureManager(const TextureManager&) = delete;
	TextureManager& operator=(const TextureManager&) = delete;

	//デバイス
	ID3D12Device* device_;
	//ディレクトリパス
	std::string directoryPath_;
	//テクスチャコンテナ
	std::array<Texture, SRVDescriptorHerpManager::kNumDescriptors> textures_;

	//コンバートストリング
	std::wstring ConvertString(const std::string& str);
	//コンバートストリング
	std::string ConvertString(const std::wstring& str);

	/// <summary>
	/// テキストデータを読む
	/// </summary>
	ScratchImage LoadTexture(const std::string& filePath);
	
	/// <summary>
	/// TextureResourceを作る
	/// </summary>
	/// <param name="metadata"></param>
	/// <returns></returns>
	Microsoft::WRL::ComPtr<ID3D12Resource> CreateTextureResource(const TexMetadata& metadata);

	/// <summary>
	/// TextureResourceにデータを転送する
	/// </summary>
	Microsoft::WRL::ComPtr<ID3D12Resource> UploadTextureData(Microsoft::WRL::ComPtr<ID3D12Resource> texture, const ScratchImage& mipImages,
		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList);

	/// <summary>
	/// 読み込み
	/// </summary>
	/// <param name="fileName">ファイル名</param>
	/// <returns></returns>
	uint32_t LoadInternal(const std::string& fileName ,DirectXCommon* dxCommon);

};

