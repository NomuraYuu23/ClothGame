#pragma once
#include <DirectXMath.h>
#include <Windows.h>
#include <d3d12.h>
#include <string>
#include <wrl.h>
#include <dxcapi.h>
#include <list>
#include <memory>
#include "../3D/Vertex/VertexData.h"
#include "../3D/Material/Material.h"
#include "../Collider2D/Collider2D.h"
#include "../Collider2D/ColliderShape2D.h"
#include "../Collider2D/Collider2DDebugDrawForGPU.h"

#pragma comment(lib, "dxcompiler.lib")

class Collision2DDebugDraw
{

public:

	/// <summary>
	/// テクスチャ名
	/// </summary>
	enum TexutureName {
		kTexutureNameBox, // 矩形
		kTexutureNameCircle, // 円
		kTexutureNameOfCount // 使わない
	};


public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Collision2DDebugDraw();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="device">デバイス</param>
	/// <param name="textureHandles">テクスチャ</param>
	/// <param name="rootSignature">ルートシグネチャ</param>
	/// <param name="pipelineState">パイプライン</param>
	void Initialize(ID3D12Device* device,
		const std::array<UINT, kTexutureNameOfCount>& textureHandles,
		ID3D12RootSignature* rootSignature,
		ID3D12PipelineState* pipelineState);

	/// <summary>
	/// クリア
	/// </summary>
	void Clear();

	/// <summary>
	/// 登録
	/// </summary>
	void Register(ColliderShape2D collider);

	/// <summary>
	/// コマンドリスト
	/// </summary>
	/// <param name="cmdList">コマンドリスト</param>
	void Draw(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// ImGui描画
	/// </summary>
	void ImGuiDraw();

private:

	/// <summary>
	/// SRVを作る
	/// </summary>
	void SRVCreate();

private:

	//デバイス
	ID3D12Device* device_;
	// コマンドリスト
	ID3D12GraphicsCommandList* commandList_;
	// ルートシグネチャ
	ID3D12RootSignature* rootSignature_;
	// パイプラインステートオブジェクト
	ID3D12PipelineState* pipelineState_;
	// 非表示
	bool isInvisible_;

	// ビュー行列
	Matrix4x4 viewMatrix_;
	// プロジェクション行列
	Matrix4x4 projectionMatrix_;

private: // 頂点

	// 頂点バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff_;
	// 頂点バッファマップ
	VertexData* vertMap = nullptr;
	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView_{};

	Microsoft::WRL::ComPtr<ID3D12Resource> indexBuff_;

	//インデックスバッファビュー
	D3D12_INDEX_BUFFER_VIEW ibView_{};

	//インデックスリソースにデータを書き込む
	uint32_t* indexMap = nullptr;

private: // テクスチャ

	//テクスチャ番号
	std::array<UINT, kTexutureNameOfCount> textureHandles_;
	// リソース設定
	std::array<D3D12_RESOURCE_DESC, kTexutureNameOfCount> resourceDesc_;

	const Vector2 textureScale = { 16.0f, 16.0f};

private: // トランスフォーム

	//WVP用のリソースを作る。
	Microsoft::WRL::ComPtr<ID3D12Resource> collider2DDebugDrawForGPUBuff_;
	//書き込むためのアドレスを取得
	Collider2DDebugDrawForGPU* collider2DDebugDrawForGPUMap_{};
	// 最大数
	const uint32_t kCollider2DDebugDrawForGPUNumMax_ = 32;
	// 数
	uint32_t collider2DDebugDrawForGPUNumCount_ = 0;
	// CPUハンドル
	D3D12_CPU_DESCRIPTOR_HANDLE instancingSrvHandleCPU_;
	// GPUハンドル
	D3D12_GPU_DESCRIPTOR_HANDLE instancingSrvHandleGPU_;
	// ディスクリプタヒープの位置
	uint32_t indexDescriptorHeap_ = 0;

private: // マテリアル

	std::unique_ptr<Material> material_;

};

