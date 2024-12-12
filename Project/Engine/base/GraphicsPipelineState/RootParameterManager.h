#pragma once
#include <d3d12.h>
#include <string>
#include <dxcapi.h>
#include <cassert>
#include <format>
#include <wrl.h>
#include <array>
#include <vector>
#include <variant>

/// <summary>
/// SRV PS
/// </summary>
enum SRV_PSIndex {
	kSRV_PSIndexTexture0 = 0, // テクスチャ0
	kSRV_PSIndexTexture1 = 1, // テクスチャ1
	kSRV_PSIndexTexture2 = 2, // テクスチャ2
	kSRV_PSIndexTexture3 = 3, // テクスチャ3
	kSRV_PSIndexTexture4 = 4, // テクスチャ4
	kSRV_PSIndexTexture5 = 5, // テクスチャ5
	kSRV_PSIndexTexture6 = 6, // テクスチャ6
	kSRV_PSIndexTexture7 = 7, // テクスチャ7
	kSRV_PSIndexPointLight = 8, // ポイントライト
	kSRV_PSIndexSpotLight = 9, // スポットライト
	kSRV_PSIndexMaterials = 10, // マテリアル
	kSRV_PSIndexEnvironmentTexture = 11, // 環境マップ(映り込み用テクスチャ)
	kSRV_PSIndexDissolve = 12, // Dissolveデータ(float)
	kSRV_PSIndexOfCount,
};

/// <summary>
/// SRV VS
/// </summary>
enum SRV_VSIndex {
	kSRV_VSIndexParticleForGPU = 0, //  パーティクル用
	kSRV_VSIndexCollider2DDebugDrawForGPU = 1, // コライダーデバッグ2d
	kSRV_VSIndexTransformationMatrix = 2, // トランスフォーム行列
	kSRV_VSIndexLineForGPU = 3, // 線描画
	kSRV_VSIndexGPUParticleForGPU = 4, //  GPUパーティクル用
	kSRV_VSIndexVertex= 5, // 頂点
	kSRV_VSIndexOfCount,
};

/// <summary>
/// CBV PS
/// </summary>
enum CBV_PSIndex {

	kCBV_PSIndexMaterial = 0,// マテリアル
	kCBV_PSIndexDirectionalLight = 1,// 平行光源
	kCBV_PSIndexCamera = 2,// カメラ
	kCBV_PSIndexFog = 3,// 霧
	kCBV_PSIndexOfCount

};

/// <summary>
/// CBV VS
/// </summary>
enum CBV_VSIndex {

	kCBV_VSIndexWorldTransform = 0,// ワールドトランスフォーム

	kCBV_VSIndexParticleStart = 2,// パーティクルの開始位置
	kCBV_VSIndexViewProjection = 3,// viewProjection
	kCBV_VSIndexSpriteForGPU = 4, // スプライト用
	kCBV_VSIndexWVP = 5, // WVP用
	kCBV_VSIndexGPUParticleView = 6, //  GPUパーティクルのView
	kCBV_VSIndexOfCount

};

/// <summary>
/// UAV PS
/// </summary>
enum UAV_PSIndex {
	kUAV_PSIndexTexture0 = 0, // テクスチャ0
	kUAV_PSIndexOfCount
};


/// <summary>
/// UAV VS
/// </summary>
enum UAV_VSIndex {
	kUAV_VSIndexVertex = 0, // 頂点
	kUAV_VSIndexOfCount
};

/// <summary>
/// ルートパラメータの名前
/// </summary>
enum RootParameterIndex {
	kRootParameterIndexModel, // アニメーション無しモデル
	kRootParameterIndexAnimModel, // アニメーションありモデル
	kRootParameterIndexManyModels, // たくさんのアニメーション無しモデル
	kRootParameterIndexManyAnimModels, // たくさんのアニメーション無しモデル

	kRootParameterIndexSprite, // スプライト
	kRootParameterIndexParticle, // パーティクル
	kRootParameterIndexGPUParticle, // GPUパーティクル
	kRootParameterIndexGPUParticleDissolve, // GPUパーティクルdissolveあり
	kRootParameterIndexCollision2DDebugDraw, // コライダーデバッグ2d
	kRootParameterIndexLine, // 線
	kRootParameterIndexWindowSpriteSRV, // ウィンドウスプライトSRV
	
	kRootParameterIndexSkyBox, // スカイボックス
	kRootParameterIndexCloth, // 布
	kRootParameterIndexClothGPU, // 布GPU

	kRootParameterIndexOfCount,
};

/// <summary>
/// ルートパラメータ
/// </summary>
class RootParameterManager
{

private: // サブクラス

	// ルートパラメータ種類
	using Item = std::variant<SRV_PSIndex, SRV_VSIndex, CBV_PSIndex, CBV_VSIndex, UAV_PSIndex, UAV_VSIndex>;

public: // メンバ関数

	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns></returns>
	static RootParameterManager* GetInstance();

	/// <summary>
	/// 初期化
	/// </summary>
	static void Initialize();

private: // メンバ関数

	/// <summary>
	/// ディスクリプタレンジの初期化
	/// </summary>
	static void DescriptorRangeInitialize();

	/// <summary>
	/// データ解析
	/// </summary>
	/// <param name="rootParameterIndex">ルートパラメータ番号</param>
	/// <param name="data">データ</param>
	static void Analyze(RootParameterIndex rootParameterIndex, const std::vector<Item>& data);

public: // 変数

	// ルートパラメータ
	static std::array<std::vector<D3D12_ROOT_PARAMETER>, kRootParameterIndexOfCount> rootParameters_;
	// ディスクリプタレンジ SRV PS
	static std::array<std::vector<D3D12_DESCRIPTOR_RANGE>, kSRV_PSIndexOfCount> descriptorRangesSRV_PS_;
	// ディスクリプタレンジ SRV VS
	static std::array<std::vector<D3D12_DESCRIPTOR_RANGE>, kSRV_VSIndexOfCount> descriptorRangesSRV_VS_;
	// ディスクリプタレンジ UAV PS
	static std::array<std::vector<D3D12_DESCRIPTOR_RANGE>, kUAV_PSIndexOfCount> descriptorRangesUAV_PS_;
	// ディスクリプタレンジ UAV VS
	static std::array<std::vector<D3D12_DESCRIPTOR_RANGE>, kUAV_VSIndexOfCount> descriptorRangesUAV_VS_;

private: // シングルトン

	RootParameterManager() = default;
	~RootParameterManager() = default;
	RootParameterManager(const RootParameterManager&) = delete;
	RootParameterManager& operator=(const RootParameterManager&) = delete;

};

