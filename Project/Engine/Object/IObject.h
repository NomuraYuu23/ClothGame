#pragma once
#include <string>
#include <Windows.h>
#include <chrono>
#include <cstdlib>
#include <d3d12.h>
#include "../../externals/DirectXTex/d3dx12.h"
#include <dxgi1_6.h>
#include <wrl.h>

#include "../3D/Transform/WorldTransform.h"

class IObject
{

protected: 

	static ID3D12GraphicsCommandList* commandList_;

	static uint32_t newSerialNumber_;

public:

	/// <summary>
	/// 静的初期化
	/// </summary>
	/// <param name="commandList">コマンドリスト</param>
	static void StaticInitialize(ID3D12GraphicsCommandList* commandList);

	/// <summary>
	/// シリアルナンバー初期化
	/// </summary>
	static void ResetNewSerialNumber();

public: // 関数

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~IObject() = default;
	
	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// ImGui描画
	/// </summary>
	virtual void ImGuiDraw() {};

	/// <summary>
	/// 名前取得
	/// </summary>
	/// <returns></returns>
	std::string GetName() { return name_; }

	void SetIsDead(bool isDead) { isDead_ = isDead; }
	bool GetIsDead() { return isDead_; }

	uint32_t GetSerialNumber() { return serialNumber_; }

	WorldTransform* GetWorldTransformAdress() { return &worldTransform_; }

protected: // 変数

	// 名前
	std::string name_;

	// トランスフォーム
	WorldTransform worldTransform_;

	// 死んでいるか
	bool isDead_ = false;

	//衝突属性(自分)
	uint32_t collisionAttribute_ = 0x00000000;

	// 衝突マスク(相手)
	uint32_t collisionMask_ = 0xffffffff;

	// シリアルナンバー
	uint32_t serialNumber_ = 0;

};

