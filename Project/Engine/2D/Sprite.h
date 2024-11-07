#pragma once

#include <DirectXMath.h>
#include <Windows.h>
#include <d3d12.h>
#include <string>
#include <wrl.h>
#include <dxcapi.h>
#include <list>
#include <memory>

#pragma comment(lib, "dxcompiler.lib")

#include "../Math/Vector/Vector2.h"
#include "../Math/Vector/Vector3.h"
#include "../Math/Vector/Vector4.h"
#include "../Math/Matrix/Matrix4x4.h"

#include "../2D/SpriteVertex.h"
#include "../3D/Transform/TransformationMatrix.h"
#include "../3D/Transform/TransformStructure.h"

#include "../3D/Material/Material.h"
#include "SpriteForGPU.h"

/// <summary>
/// スプライト
/// </summary>
class Sprite
{

public:

	/// <summary>
	/// 静的初期化
	/// </summary>
	/// <param name="device">デバイス</param>
	static void StaticInitialize(
		ID3D12Device* device,
		ID3D12RootSignature* rootSignature,
		ID3D12PipelineState* pipelineState);

	/// <summary>
	/// 静的前処理
	/// </summary>
	/// <param name="cmdList">描画コマンドリスト</param>
	static void PreDraw(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// 描画後処理
	/// </summary>
	static void PostDraw();

	/// <summary>
	/// スプライト生成
	/// </summary>
	/// <param name="textureHandle">テクスチャハンドル</param>

	/// <returns>生成されたスプライト</returns>
	static Sprite* Create(
		uint32_t textureHandle, const Vector2& position, const Vector4& color);

private:

	// 頂点数
	static const int kVertNum = 6;
	// デバイス
	static ID3D12Device* sDevice;
	// コマンドリスト
	static ID3D12GraphicsCommandList* sCommandList;
	// ルートシグネチャ
	static ID3D12RootSignature* sRootSignature;
	// パイプラインステートオブジェクト
	static ID3D12PipelineState* sPipelineState;

public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Sprite();
	/// <summary>
	/// コンストラクタs
	/// </summary>
	Sprite(uint32_t textureHandle, const Vector2& position, const Vector2& size, const Vector4& color);

	/// <summary>
	/// 初期化
	/// </summary>
	/// <returns>成否</returns>
	bool Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// テクスチャハンドルの設定
	/// </summary>
	/// <param name="textureHandle"></param>
	void SetTextureHandle(uint32_t textureHandle);

	uint32_t GetTevtureHandle() { return textureHandle_; }

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// テクスチャの範囲
	/// </summary>
	void SetTextureRange();
	
	/// <summary>
	/// トランスフォームマトリックス更新
	/// </summary>
	void TransformMatrixUpdate();

private:
	// 頂点バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff_;
	// 頂点バッファマップ
	SpriteVertex* vertMap = nullptr;
	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView_{};

	Microsoft::WRL::ComPtr<ID3D12Resource> indexBuff_;

	//インデックスバッファビュー
	D3D12_INDEX_BUFFER_VIEW ibView_{};

	//インデックスリソースにデータを書き込む
	uint32_t* indexMap = nullptr;

	//テクスチャ番号
	UINT textureHandle_ = 0;
	// リソース設定
	D3D12_RESOURCE_DESC resourceDesc_;

public: // アクセッサ(ワールドトランスフォーム)

	void SetPosition(const Vector2& position);

	void SetRotate(float rotate);

	void SetSize(const Vector2& size);

	void SetAnchorPoint(const Vector2& anchorPoint);

	void SetIsFlipX(bool isFlipX);

	void SetIsFlipY(bool isFlipY);
	
	void SetIsInvisible(bool isInvisible) { isInvisible_ = isInvisible; }
	
	void SetTextureSize(const Vector2& textureSize);

	void SetTextureLeftTop(const Vector2& textureLeftTop);

	// 位置
	Vector2 GetPosition() { return position_; }
	// 回転
	float GetRotate() { return rotate_; }
	// サイズ
	Vector2 GetSize() { return size_; }

	// アンカーポイント
	Vector2 GetAnchorPoint() { return anchorPoint_; }

	// フリップ
	bool GetIsFlipX() { return isFlipX_; }
	bool GetIsFlipY() { return isFlipY_; }

	// テクスチャのサイズ
	Vector2 GetTextureSize() { return textureSize_; }
	// 描画する
	Vector2 GetTextureLeftTop() { return textureLeftTop_; }
	// テクスチャ初期サイズ
	Vector2 GetTextureInitSize() { return textureInitSize_; }

	bool GetIsInvisible() { return isInvisible_; }

private: // メンバ変数(ワールドトランスフォーム)

	// 位置
	Vector2 position_;
	// 回転
	float rotate_;
	// サイズ
	Vector2 size_;

	//WVP用のリソースを作る。
	Microsoft::WRL::ComPtr<ID3D12Resource> spriteForGPUBuff_;
	//書き込むためのアドレスを取得
	SpriteForGPU* spriteForGPUMap_{};

	// ワールド行列
	Matrix4x4 transformMatrix_;

	// アンカーポイント
	Vector2 anchorPoint_;

	// フリップ
	bool isFlipX_ = false;
	bool isFlipY_ = false;

	// 非表示フラグ
	bool isInvisible_ = false;

	// テクスチャのサイズ
	Vector2 textureSize_;
	// 描画する
	Vector2 textureLeftTop_;
	// テクスチャ初期サイズ
	Vector2 textureInitSize_;

public: // アクセッサ(マテリアル)

	void SetUvTransform(const EulerTransform& uvTransform);
	void SetUvTransform(const Vector3& scale, const Vector3& rotate, const Vector3& translate);

	void SetColor(const Vector4& color);

	void SetEnableLighting(int enableLighting);

private: // メンバ変数(マテリアル)

	// マテリアル
	std::unique_ptr<Material> material_;

	EulerTransform uvTransform_;
	Vector4 color_;
	int enableLighting_;

};

