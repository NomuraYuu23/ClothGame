#include "MyGame.h"

using namespace NomeEngine;

void MyGame::Initialize()
{

	// 基底クラスの初期化
	MyFramework::Initialize();

	//ゲームシーン
	sceneManager = std::make_unique<SceneManager>();
	sceneManager->Initialize(kDebug);

}

void MyGame::Finalize()
{

	// 基底クラスの終了処理
	MyFramework::Finalize();

}

void MyGame::Update()
{

	MyFramework::Update();

	// ゲームシーン更新
	sceneManager->Update();

}

void MyGame::Draw()
{

	//描画前処理
	dxCommon->PreDraw();

	//ゲームシーン描画処理
	sceneManager->Draw();

	imGuiManager->End();

	//描画後処理
	dxCommon->PostDraw();

}
