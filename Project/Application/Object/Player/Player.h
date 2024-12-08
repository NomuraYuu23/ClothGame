#pragma once
#include "../../../Engine/Object/MeshObject.h"
#include "../../../Engine/Animation/Animation.h"
#include "State/IPlayerState.h"
#include "State/System/PlayerStateFactory.h"
#include "State/System/PlayerStateSystem.h"
#include "Animation/PlayerAnimation.h"
#include "../../GPUParticle/RunDustParticle/RunDustParticle.h"

class Player :
    public MeshObject
{

private: 

    // 足までの位置
    static const Vector3 kPositionToFeet_;

public: // メンバ関数

    /// <summary>
    /// コンストラクタ
    /// </summary>
    Player();

    /// <summary>
    /// デストラクタ
    /// </summary>
    ~Player() override;

    /// <summary>
    /// 初期化
    /// </summary>
    /// <param name="data">メッシュデータ</param>
    void Initialize(LevelData::MeshData* data) override;

    /// <summary>
    /// 更新処理
    /// </summary>
    void Update() override;

    /// <summary>
    /// 描画
    /// </summary>
    /// <param name="camera">カメラ</param>
    void Draw(BaseCamera& camera) override;

    /// <summary>
    /// ImGui描画
    /// </summary>
    void ImGuiDraw() override;

    /// <summary>
    /// パーティクル描画
    /// </summary>
    /// <param name="camera">カメラ</param>
    void ParticleDraw(BaseCamera& camera) override;

    /// <summary>
    /// 衝突処理
    /// </summary>
    /// <param name="colliderPartner"></param>
    /// <param name="collisionData"></param>
    void OnCollision(ColliderParentObject colliderPartner, const CollisionData& collisionData);

    /// <summary>
    /// ダメージ処理
    /// </summary>
    void Damage();

private: // メンバ関数

	/// <summary>
	/// コライダー更新
	/// </summary>
	void ColliderUpdate();

    /// <summary>
    /// 落下確認
    /// </summary>
    void FallCheck();

public: // アクセッサ

    /// <summary>
    /// プレイヤーが浮いているか取得
    /// </summary>
    /// <returns></returns>
    bool GetFloating() { return floating_; }

    /// <summary>
    /// プレイヤーが浮いているか設定
    /// </summary>
    /// <returns></returns>
    void SetFloating(bool floating) { floating_ = floating; }

    /// <summary>
    /// ワープ設定
    /// </summary>
    /// <param name="warping">ワープ</param>
    void SetWarping(bool warping) { warping_ = warping; }

    /// <summary>
    /// ワープ取得
    /// </summary>
    /// <returns></returns>
    bool GetWarping() { return warping_; }

    /// <summary>
    /// レベルアップ設定
    /// </summary>
    /// <param name="warping">レベルアップ</param>
    void SetLevelUp(bool levelUp) { levelUp_ = levelUp; }

    /// <summary>
    /// レベルアップ取得
    /// </summary>
    /// <returns></returns>
    bool GetLevelUp() { return levelUp_; }

    /// <summary>
    /// 現在のステート番号
    /// </summary>
    /// <returns></returns>
    uint32_t GetCurrentStateNo() { return playerStateSystem_->GetCurrentStateNo(); };

private: // メンバ変数

    // ステートシステム
    std::unique_ptr<PlayerStateSystem> playerStateSystem_;

    // アニメーション
    std::unique_ptr<PlayerAnimation> playerAnimation_;

    // プレイヤーが浮いているか
    bool floating_;

    // ワープ
    bool warping_;

    // レベルアップ
    bool levelUp_;

    // 砂ぼこり
    std::unique_ptr<RunDustParticle> runDustParticle_;

};

