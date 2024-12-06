#pragma once
#include "../../../Engine/3D/ManyObjects/LargeNumberOfObjects.h"
#include "../../../Engine/Object/MeshObject.h"

class ShadowManager :
    public LargeNumberOfObjects
{

public:

	/// <summary>
	/// 影候補
	/// </summary>
	struct ShadowCandidate {
		Vector3 position_; // 位置
		Vector3 size_; // 大きさ,コライダーサイズいれとけばおｋ
	};

public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	void Initialize(Model* model);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 前更新
	/// </summary>
	void PreUpdate();

	/// <summary>
	/// 影を発生させるオブジェクトリスト登録
	/// </summary>
	/// <param name="object">オブジェクト</param>
	/// <param name="sizeAdd">大きさ加算分</param>
	void CastsShadowObjListRegister(MeshObject* object, const Vector3& sizeAdd = {0.0f,0.0f,0.0f});

	/// <summary>
	/// 影が現れるオブジェクトリスト登録
	/// </summary>
	/// <param name="object">オブジェクト</param>
	/// <param name="sizeAdd">大きさ加算分</param>
	void ShadowAppearsObjListRegister(MeshObject* object, const Vector3& sizeAdd = { 0.0f,0.0f,0.0f });

private:

	/// <summary>
	/// 影が見えるか
	/// </summary>
	void SeeShadow();

	/// <summary>
	/// 影候補が衝突しているか
	/// </summary>
	/// <param name="castsShadow">影を発生させるオブジェクト</param>
	/// <param name="shadowAppears">影が現れるオブジェクト</param>
	/// <returns>衝突したか</returns>
	bool CollisionCheck(const ShadowCandidate& castsShadow, const ShadowCandidate& shadowAppears);

	/// <summary>
	/// 内包時衝突処理
	/// </summary>
	/// <param name="castsShadow">影を発生させるオブジェクト</param>
	/// <param name="shadowAppears">影が現れるオブジェクト</param>
	void CompriseOnCollision(const ShadowCandidate& castsShadow, const ShadowCandidate& shadowAppears);

	/// <summary>
	/// 非内包時衝突処理
	/// </summary>
	/// <param name="castsShadow">影を発生させるオブジェクト</param>
	/// <param name="shadowAppears">影が現れるオブジェクト</param>
	void NotCompriseOnCollision(const ShadowCandidate& castsShadow, const ShadowCandidate& shadowAppears);

	/// <summary>
	/// 影の個数制限
	/// </summary>
	void ShadowLimit();

private:

	// 影を発生させるオブジェクトリスト
	std::list<ShadowCandidate> castsShadowObjList_;

	// 影が現れるオブジェクトリスト
	std::list<ShadowCandidate> shadowAppearsObjList_;

	// 影を出す数
	uint32_t shadowCount_;

	// 影が最大か
	bool isShadowMax_;

	// 影の位置追加分
	float posYAdd_;

};

