#pragma once
#include "../../Math/Vector/Vector3.h"
#include "../../Math/Vector/Vector2.h"
#include "../../3D/Line/DrawLine.h"
#include "ClothModel.h"

/// <summary>
/// 布CPU
/// </summary>
class Cloth
{

public: // サブクラス

	/// <summary>
	/// バネの種類
	/// </summary>
	enum TypeOfSpring {
		StructuralSpring = 0, // 構成バネ
		ShearSpring = 1, // せん断バネ
		BendingSpring = 2, // 曲げバネ
	};

	/// <summary>
	/// 布用質点
	/// </summary>
	struct ClothMassPoint
	{
		Vector3 position; // 現在位置
		Vector3 prePosition; // 前フレ―ムの位置
		float weight; // 運動計算の重み (固定する場合は0.0f, それ以外は1.0f)
		uint32_t y; // y 検索用
		uint32_t x; // x 検索用
	};
	
	/// <summary>
	/// 布用バネ
	/// </summary>
	struct ClothSpring
	{
		ClothMassPoint* point0; // 質点0
		ClothMassPoint* point1; // 質点1
		float naturalLength; // 自然長
		TypeOfSpring type; // バネの種類
	};

public: // メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="scale">大きさ</param>
	/// <param name="div">分割数</param>
	void Initialize(const Vector2& scale, const Vector2& div);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="commandList">コマンドリスト</param>
	/// <param name="camera">カメラ</param>
	void Draw(ID3D12GraphicsCommandList* commandList, BaseCamera* camera);

	/// <summary>
	/// デバッグ描画用マッピング
	/// </summary>
	/// <param name="drawLine">線描画ポインタ</param>
	void DebugDrawMap(DrawLine* drawLine);

	/// <summary>
	/// ImGui
	/// </summary>
	void ImGuiDraw();

public: // 外部操作関数

	/// <summary>
	/// 重み設定
	/// </summary>
	/// <param name="y">y</param>
	/// <param name="x">x</param>
	/// <param name="isWight">重みをつけるか</param>
	void SetWeight(uint32_t y, uint32_t x, bool isWight);

	/// <summary>
	/// 位置設定
	/// </summary>
	/// <param name="y">y</param>
	/// <param name="x">x</param>
	/// <param name="position">位置</param>
	void SetPosition(uint32_t y, uint32_t x, const Vector3& position);

private: // 初期化関数

	/// <summary>
	/// 質点初期化
	/// </summary>
	void MassPointsInitialize();

	/// <summary>
	/// バネ初期化
	/// </summary>
	void SpringInitialize();

private: // 更新関数

	/// <summary>
	/// 積分フェーズ
	/// </summary>
	void IntegralPhase();

	/// <summary>
	/// バネフェーズ
	/// </summary>
	void SpringPhase();

private: // その他関数

	/// <summary>
	/// バネ作成
	/// </summary>
	/// <param name="x">始点X</param>
	/// <param name="y">始点Y</param>
	/// <param name="offsetX">始点から終点への距離X</param>
	/// <param name="offsetY">始点から終点への距離Y</param>
	/// <param name="type">タイプ</param>
	void SpringGeneration(
		uint32_t x, 
		uint32_t y, 
		int32_t offsetX, 
		int32_t offsetY, 
		TypeOfSpring type);

private: // メンバ変数

	Vector2 scale_; // 大きさ
	Vector2 div_; // 分割数
	std::vector<ClothMassPoint> massPoints_; // 質点
	std::vector<ClothSpring> springs_; // バネ

	float mass = 1.0f; // 質点の質量
	float stiffness_; // 剛性。バネ定数k

	float speedResistance_; // 速度抵抗

	int32_t relaxation_; // バネフェーズの反復回数
	
	float structuralShrink_; // 構成バネ伸び抵抗
	float structuralStretch_; // 構成バネ縮み抵抗
	float shearShrink_; // せん断バネ伸び抵抗
	float shearStretch_; // せん断バネ縮み抵抗
	float bendingShrink_; // 曲げバネ伸び抵抗
	float bendingStretch_; // 曲げバネ縮み抵抗

	float step_; // 1フレーム

	bool structuralDebugDraw_; // 構成バネデバッグ描画
	bool shearDebugDraw_; // せん断バネデバッグ描画
	bool bendingDebugDraw_; // 曲げバネデバッグ描画

	ClothModel model_; // モデル

};

