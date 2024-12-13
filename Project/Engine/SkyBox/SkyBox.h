#pragma once
#include <DirectXMath.h>
#include <Windows.h>
#include <d3d12.h>
#include <string>
#include <wrl.h>
#include <dxcapi.h>

#pragma comment(lib, "dxcompiler.lib")

#include "../Math/Vector/Vector4.h"
#include "../Math/Matrix/Matrix4x4.h"
#include "../3D/Material/Material.h"
#include "../Camera/BaseCamera.h"

/// <summary>
/// スカイボックス
/// </summary>
class Skybox
{

public: // サブクラス

	/// <summary>
	/// 頂点データ
	/// </summary>
	struct VertexData {
		Vector4 position; // 位置
	};

	/// <summary>
	/// GPUに送るデータ
	/// </summary>
	struct SkyboxForGPU {
		Matrix4x4 WVP; // ワールドビュープロジェクション
	};

public: // 関数

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="textureHandle">テクスチャハンドル</param>
	/// <param name="rootSignature">ルートシグネチャ</param>
	/// <param name="pipelineState">パイプライン</param>
	void Initialize(uint32_t textureHandle, ID3D12RootSignature* rootSignature, ID3D12PipelineState* pipelineState);

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="commandList">コマンドリスト</param>
	/// <param name="camera">カメラ</param>
	void Draw(ID3D12GraphicsCommandList* commandList, BaseCamera* camera);

private: // 関数

	/// <summary>
	/// 頂点マッピング
	/// </summary>
	void VertMapping();

	/// <summary>
	/// インデックスマッピング
	/// </summary>
	void IndexMapping();

public: // アクセッサ

	/// <summary>
	/// トランスフォーム
	/// </summary>
	/// <param name="transform"></param>
	void SetTransform(const EulerTransform& transform) { transform_ = transform; }

private: // 定数

	// 頂点数
	static const size_t kVertNum_;
	// index数
	static const size_t kIndexNum_;

private: // 変数

	// 頂点バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff_;
	// 頂点バッファマップ
	Skybox::VertexData* vertMap_ = nullptr;
	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView_{};

	// インデックスバッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> indexBuff_;
	//インデックスマップ
	uint32_t* indexMap_ = nullptr;
	//インデックスバッファビュー
	D3D12_INDEX_BUFFER_VIEW ibView_{};

	// トランスフォームバッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> skyboxForGPUBuff_;
	// トランスフォーム行列マップ
	Skybox::SkyboxForGPU* skyboxForGPUMap_;

	// トランスフォーム行列
	Matrix4x4 transformMatrix_;

	// トランスフォーム
	EulerTransform transform_;

	// マテリアル
	std::unique_ptr<Material> material_;

	// テクスチャハンドル
	uint32_t textureHandle_;

	// ルートシグネチャ
	ID3D12RootSignature* rootSignature_;

	// パイプライン
	ID3D12PipelineState* pipelineState_;

};

