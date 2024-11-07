#pragma once
#include <vector>
#include "../Spring/StructuralSpring.h"
#include "../../3D/Line/DrawLine.h"
#include "../../3D/Model/Model.h"

class String
{

public: // 定数

	// 余分な行列
	static const uint32_t kExtraMatrixNum = 2;

public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="anchor">アンカー</param>
	/// <param name="naturalLength">自然長</param>
	/// <param name="stiffness">剛性。バネ定数k</param>
	/// <param name="dampingCoefficient">減衰係数</param>
	/// <param name="mass">質量(質点)</param>
	/// <param name="distance">距離</param>
	/// <param name="springNum">バネの数</param>
	void Initialize(
		const Vector3& anchor,
		float naturalLength,
		float stiffness,
		float dampingCoefficient,
		float mass,
		const Vector3& distance,
		uint32_t springNum);

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="anchor">アンカー</param>
	/// <param name="naturalLength">自然長</param>
	/// <param name="stiffness">剛性。バネ定数k</param>
	/// <param name="dampingCoefficient">減衰係数</param>
	/// <param name="mass">質量(質点)</param>
	void Initialize(
		Model* model,
		const Vector3& anchor,
		float naturalLength,
		float stiffness,
		float dampingCoefficient,
		float mass);

	/// <summary>
	/// 更新
	/// </summary>
	void Update(const Vector3& wind = Vector3{ 0.0f,0.0f,0.0f },
		const Vector3& gravity = Vector3{ 0.0f,-9.8f,0.0f });

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="camera">カメラ</param>
	void Draw(BaseCamera& camera);

	/// <summary>
	/// デバッグ描画用マッピング
	/// </summary>
	/// <param name="drawLine">線描画ポインタ</param>
	void DebugDrawMap(DrawLine* drawLine);

	/// <summary>
	/// アンカー設定
	/// </summary>
	/// <param name="pointIndex">インデックス</param>
	/// <param name="fixPoint">固定するか</param>
	void SetAnchor(uint32_t pointIndex, bool fixPoint);

	/// <summary>
	/// 位置設定
	/// </summary>
	/// <param name="pointIndex">インデックス</param>
	/// <param name="position">位置</param>
	void SetPosition(uint32_t pointIndex, const Vector3& position);
	
public: // アクセッサ

	std::vector<StructuralSpring> GetSpring() { return structuralSpring_; }

private: // 変数

	//バネ
	std::vector<StructuralSpring> structuralSpring_;

private: // 変数(モデル)

	// モデル
	Model* model_ = nullptr;
	// マテリアル
	std::unique_ptr<Material> material_ = nullptr;
	// ワールドトランスフォーム
	WorldTransform worldTransform_{};
	// ローカル行列
	std::unique_ptr<LocalMatrixManager> localMatrixManager_ = nullptr;

};

