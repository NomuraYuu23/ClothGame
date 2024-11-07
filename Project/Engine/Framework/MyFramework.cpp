#include "MyFramework.h"

void MyFramework::Initialize()
{

	//ゲームウィンドウの作成
	win = WinApp::GetInstance();
	win->CreateGameWindow(L"NomuEngine");

	//DirectX初期化
	dxCommon = DirectXCommon::GetInstance();
	dxCommon->Initialize(win);

	//入力デバイス
	input = Input::GetInstance();
	input->Initialize(win->GetHInstance(), win->GetHwnd());

	//テクスチャマネージャー
	TextureManager::GetInstance()->Initialize(dxCommon->GetDevice());

	// マテリアル静的初期化
	Material::StaticInitialize(dxCommon->GetDevice());

	// スプライト静的初期化
	Sprite::StaticInitialize(dxCommon->GetDevice(), 
		GraphicsPipelineState::sRootSignature[GraphicsPipelineState::kPipelineStateIndexSprite].Get(), 
		GraphicsPipelineState::sPipelineState[GraphicsPipelineState::kPipelineStateIndexSprite].Get());

	// Windowサイズのスプライト
	WindowSprite::GetInstance()->Initialize(dxCommon->GetDevice(), dxCommon->GetCommadList());

	// モデル静的初期化
	std::array<ID3D12RootSignature*, ModelDraw::PipelineStateIndex::kPipelineStateIndexOfCount> rootSignature = {
		GraphicsPipelineState::sRootSignature[GraphicsPipelineState::kPipelineStateIndexModel].Get(),
		GraphicsPipelineState::sRootSignature[GraphicsPipelineState::kPipelineStateIndexAnimModel].Get(),
		GraphicsPipelineState::sRootSignature[GraphicsPipelineState::kPipelineStateIndexAnimInverseModel].Get(),
		GraphicsPipelineState::sRootSignature[GraphicsPipelineState::kPipelineStateIndexManyModels].Get(),
		GraphicsPipelineState::sRootSignature[GraphicsPipelineState::kPipelineStateIndexAnimManyModels].Get(),
		GraphicsPipelineState::sRootSignature[GraphicsPipelineState::kPipelineStateIndexAnimModelRT2].Get(),
		GraphicsPipelineState::sRootSignature[GraphicsPipelineState::kPipelineStateIndexAnimInverseModelRT2].Get() };

	std::array<ID3D12PipelineState*, ModelDraw::PipelineStateIndex::kPipelineStateIndexOfCount> pipelineState = {
		GraphicsPipelineState::sPipelineState[GraphicsPipelineState::kPipelineStateIndexModel].Get(),
		GraphicsPipelineState::sPipelineState[GraphicsPipelineState::kPipelineStateIndexAnimModel].Get(),
		GraphicsPipelineState::sPipelineState[GraphicsPipelineState::kPipelineStateIndexAnimInverseModel].Get(),
		GraphicsPipelineState::sPipelineState[GraphicsPipelineState::kPipelineStateIndexManyModels].Get(),
		GraphicsPipelineState::sPipelineState[GraphicsPipelineState::kPipelineStateIndexAnimManyModels].Get(),
		GraphicsPipelineState::sPipelineState[GraphicsPipelineState::kPipelineStateIndexAnimModelRT2].Get(),
		GraphicsPipelineState::sPipelineState[GraphicsPipelineState::kPipelineStateIndexAnimInverseModelRT2].Get() };
	
	Model::StaticInitialize(dxCommon->GetDevice());
	ModelManager::GetInstance()->Initialize(dxCommon);

	// モデル描画
	ModelDraw::Initialize(rootSignature, pipelineState);

	// 光源静的初期化
	DirectionalLight::StaticInitialize(dxCommon->GetDevice());
	PointLightManager::StaticInitialize(dxCommon->GetDevice());
	SpotLightManager::StaticInitialize(dxCommon->GetDevice());

	// GPUパーティクル
	GPUParticle::StaticInitialzie();

	// ポストエフェクト
	PostEffect::GetInstance()->Initialize();

	// 霧
	FogManager::GetInstance()->Initialize();

	// オブジェクト
	IObject::StaticInitialize(dxCommon->GetCommadList());

#ifdef _DEMO

	// クエリタイムスタンプ
	QueryTimestamp::GetInstance()->Initialize(dxCommon->GetDevice());

#endif // _DEMO

	//サウンド
	audio = Audio::GetInstance();
	audio->Initialize();

	// ImGuiマネージャー
	imGuiManager = ImGuiManager::GetInstance();
	imGuiManager->Initialize(win, dxCommon);

	//グローバル変数ファイル読み込み
	GlobalVariables::GetInstance()->LoadFiles();

	endRequst_ = false;

}

void MyFramework::Finalize()
{

	// サウンド後始末
	audio->Finalize();

	//色々な解放処理の前に書く
	imGuiManager->Finalize();

	//ゲームウィンドウの破棄
	win->TerminateGameWindow();

}

void MyFramework::Update()
{

	//Windowにメッセージが来てたら最優先で処理させる
	if (win->ProcessMessage() || input->TriggerKey(DIK_ESCAPE)) {
		endRequst_ = true;
	}

	//入力デバイス
	input->Update();
	if (!input->GetJoystickConnected()) {
		input->JoystickConnected(win->GetHwnd());
	}

	//ゲームの処理 
	//ImGui
	imGuiManager->Begin();

	//開発用UIの処理。実際に開発用のUIを出す場合はここをゲーム固有の処理に置き換える
	//ImGui::ShowDemoWindow();
	// グローバル変数の更新
	GlobalVariables::GetInstance()->Update();

#ifdef _DEMO

	// クエリタイムスタンプ
	QueryTimestamp::GetInstance()->ImGuiDraw();

#endif // _DEMO

}

void MyFramework::Run()
{

	Initialize();

	//ウィンドウののボタンが押されるまでループ
	while (true) {

		Update();
		if (IsEndRequst()) {
			break;
		}
		Draw();
	}

	Finalize();

}
