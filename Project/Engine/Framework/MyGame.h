#pragma once

// フレームワーク
#include "MyFramework.h"

class MyGame : public MyFramework
{

public: // メンバ関数

	void Initialize() override;

	void Finalize() override;

	void Update() override;

	void Draw() override;

private: // メンバ変数

	// シーンマネージャー
	std::unique_ptr<SceneManager> sceneManager = nullptr;

};

