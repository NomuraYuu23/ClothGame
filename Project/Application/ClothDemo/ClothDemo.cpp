#include "ClothDemo.h"
#include "../../Engine/2D/ImguiManager.h"

void ClothDemo::Initilalize(
	DirectionalLight* directionalLight,
	PointLightManager* pointLightManager,
	SpotLightManager* spotLightManager)
{

	// エンジン
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();

	// 布のライト
	ClothManager::GetInstance()->SetLight(
		directionalLight,
		pointLightManager,
		spotLightManager);
	clothFixedFunctions_[kFixedIndexEnd] = std::bind(&ClothDemo::ClothFixedEnd, this);
	clothFixedFunctions_[kFixedIndexTop] = std::bind(&ClothDemo::ClothFixedTop, this);

	// 布の初期の大きさ
	const Vector2 kInitClothScale = { 2.0f, 2.0f };
	clothScale_ = kInitClothScale;
	// 布の初期の分割数
	const Vector2 kInitClothDiv = { 32.0f, 32.0f };
	clothDiv_ = kInitClothDiv;
	// リセット位置
	const Vector3 kInitResetPosition = { 0.0f,3.0f,0.0f };
	resetPosition_ = kInitResetPosition;

	// 衝突オブジェクト
	// 平面
	plane_ = std::make_unique<ClothDemoPlane>();
	// 名前
	const std::string kPlane = "plane";
	plane_->Initialize(kPlane);

	// 球
	sphere_ = std::make_unique<ClothDemoSphere>();
	// 名前
	const std::string kSphere = "sphere";
	sphere_->Initialize(kSphere);

	// カプセル
	capsule_ = std::make_unique<ClothDemoCapsule>();
	// 名前
	const std::string kCapsule = "capsule";
	capsule_->Initialize(kCapsule);

	// 布の初期化
	ClothReset(dxCommon_->GetCommadListLoad());
	
	// リセット
	ClothPositionReset(kFixedIndexTop);

}

void ClothDemo::Update()
{
	// 布CPU側処理
	clothGPU_->Update();

	// 平面
	plane_->Update();
	ClothGPUCollision::CollisionDataMap planeData = plane_->GetData();
	clothGPU_->CollisionDataUpdate(plane_->GetName(), planeData);

	// 球
	sphere_->Update();
	ClothGPUCollision::CollisionDataMap sphereData = sphere_->GetData();
	clothGPU_->CollisionDataUpdate(sphere_->GetName(), sphereData);

	// カプセル
	capsule_->Update();
	ClothGPUCollision::CollisionDataMap capsuleData = capsule_->GetData();
	clothGPU_->CollisionDataUpdate(capsule_->GetName(), capsuleData);

}

void ClothDemo::Draw(BaseCamera* camera)
{

	clothGPU_->Draw(dxCommon_->GetCommadList(), camera);

}

void ClothDemo::ImGuiDraw(BaseCamera& camera)
{

	clothGPU_->ImGuiDraw("布計算パラメータ");

	ImGui::Begin("クロスシミュレーションデモ");
	// 布
	ImGui::Text("布");

	// 分割数
	const float kImGuiSpeedDiv = 1.0f;
	const float kImGuiMinDiv = 4.0f;
	const float kImGuiMaxDiv = 256.0f;
	ImGui::DragFloat2("分割数", &clothDiv_.x, kImGuiSpeedDiv, kImGuiMinDiv, kImGuiMaxDiv);
	// 分割数
	const float kImGuiSpeedScale = 0.01f;
	const float kImGuiMinScale = 1.0f;
	const float kImGuiMaxScale = 100.0f;
	ImGui::DragFloat2("大きさ", &clothScale_.x, kImGuiSpeedScale, kImGuiMinScale, kImGuiMaxScale);

	if (ImGui::Button("リセット(作成し直す)")) {
		// 布の初期化
		ClothReset(dxCommon_->GetCommadList());
		// リセット
		ClothPositionReset(kFixedIndexTop);
	}
	
	// リセット位置
	const float kImGuiSpeedResetPosition = 0.01f;
	ImGui::DragFloat3("リセット位置", &resetPosition_.x, kImGuiSpeedResetPosition);
	
	if (ImGui::Button("固定部分を解除する")) {
		RemoveFixation();
	}
	if (ImGui::Button("端4点を固定する")) {
		ClothPositionReset(kFixedIndexEnd);
	}
	if (ImGui::Button("上部分を固定する")) {
		ClothPositionReset(kFixedIndexTop);
	}

	// 物体
	std::string name = "";

	// 平面
	plane_->ImGuiDraw(camera);
	if (plane_->GetExist()) {
		name = "平面を消す";
	}
	else {
		name = "平面を出す";
	}
	if (ImGui::Button(name.c_str())) {
		PlaneSwitching();
	}
	// 球
	sphere_->ImGuiDraw(camera);
	if (sphere_->GetExist()) {
		name = "球を消す";
	}
	else {
		name = "球を出す";
	}
	if (ImGui::Button(name.c_str())) {
		SphereSwitching();
	}
	capsule_->ImGuiDraw(camera);
	if (capsule_->GetExist()) {
		name = "カプセルを消す";
	}
	else {
		name = "カプセルを出す";
	}
	if (ImGui::Button(name.c_str())) {
		CapsuleSwitching();
	}
	ImGui::End();

}

void ClothDemo::CollisionObjectDraw(BaseCamera* camera)
{

	plane_->Draw(*camera);
	sphere_->Draw(*camera);
	capsule_->Draw(*camera);

}

void ClothDemo::ClothPositionReset(FixedIndex fixedIndex)
{

	// 全ての質点の固定をはずす、リセット位置へ
	for (uint32_t y = 0; y <= static_cast<uint32_t>(clothDiv_.y); ++y) {
		for (uint32_t x = 0; x <= static_cast<uint32_t>(clothDiv_.x); ++x) {
			clothGPU_->SetWeight(y, x, true);
			clothGPU_->SetPosition(y, x, resetPosition_);
		}
	}

	clothFixedFunctions_[fixedIndex]();

}

void ClothDemo::RemoveFixation()
{

	// 全ての質点の固定をはずす
	for (uint32_t y = 0; y <= static_cast<uint32_t>(clothDiv_.y); ++y) {
		for (uint32_t x = 0; x <= static_cast<uint32_t>(clothDiv_.x); ++x) {
			clothGPU_->SetWeight(y, x, true);
		}
	}

}

void ClothDemo::ClothReset(ID3D12GraphicsCommandList* commandList)
{

	// 布の計算データ一時保存
	float mass = 1.0f; // 質量
	Vector3 gravity{}; // 重力
	Vector3 wind{}; // 風力
	float stiffness = 0.0f; // 剛性。バネ定数k
	float speedResistance = 0.0f; // 速度抵抗
	float structuralShrink = 0.0f; // 構成バネ伸び抵抗
	float structuralStretch = 0.0f; // 構成バネ縮み抵抗
	float shearShrink = 0.0f; // せん断バネ伸び抵抗
	float shearStretch = 0.0f; // せん断バネ縮み抵抗
	float bendingShrink = 0.0f; // 曲げバネ伸び抵抗
	float bendingStretch = 0.0f; // 曲げバネ縮み抵抗
	float velocityLimit = 0.0f; // 速度制限
	int32_t relaxation = 1;

	if (clothGPU_) {
		gravity = clothGPU_->GetGravity(); // 重力
		wind = clothGPU_->GetWind(); // 風力
		stiffness = clothGPU_->GetStiffness(); // 剛性。バネ定数k
		speedResistance = clothGPU_->GetSpeedResistance(); // 速度抵抗
		// 抵抗
		structuralShrink = clothGPU_->GetStructuralStretch();
		structuralStretch = clothGPU_->GetStructuralShrink();
		shearShrink = clothGPU_->GetShearStretch();
		shearStretch = clothGPU_->GetShearShrink();
		bendingShrink = clothGPU_->GetBendingStretch();
		bendingStretch = clothGPU_->GetBendingShrink();
		// 速度制限
		velocityLimit = clothGPU_->GetVelocityLimit();
		// 更新回数
		relaxation = clothGPU_->GetRelaxation();
	}
	else {
		// 初期化
		mass = 1.0f; // 質量
		gravity = {0.0f,-9.8f, 0.0f}; // 重力
		wind = { 0.0f, 0.0f, 0.0f }; // 風力
		speedResistance = 0.0f; // 速度抵抗

		// 剛性。バネ定数k
		const float kInitStiffness = 100.0f;
		stiffness = kInitStiffness;

		// 抵抗 (structural > shear >= bending)の大きさが良い
		const float kInitStructural = 100.0f; //構成
		const float kInitShear = 80.0f; // せん断
		const float kInitBending = 60.0f; // 曲げ
		structuralShrink = kInitStructural;
		structuralStretch = kInitStructural;
		shearShrink = kInitShear;
		shearStretch = kInitShear;
		bendingShrink = kInitBending;
		bendingStretch = kInitBending;
		
		// 速度制限
		const float kInitVelocityLimit = 1000.0f;
		velocityLimit = kInitVelocityLimit;
		
		// 更新回数
		const uint32_t kInitRelaxation = 6;
		relaxation = kInitRelaxation; // 最大数
	}

	// 布の初期化
	clothGPU_.reset(nullptr);
	clothGPU_ = std::make_unique<ClothGPU>();
	clothGPU_->Initialize(dxCommon_->GetDevice(), commandList, clothScale_, clothDiv_, "Resources/Sprite/Cloth/BlueCloth.dds");

	// 布の計算データを戻す
	clothGPU_->SetMass(mass); // 質量
	clothGPU_->SetGravity(gravity); // 重力
	clothGPU_->SetWind(wind); // 風力
	clothGPU_->SetStiffness(stiffness); // 剛性。バネ定数k
	clothGPU_->SetSpeedResistance(speedResistance); // 速度抵抗
	// 抵抗
	clothGPU_->SetStructuralStretch(structuralShrink);
	clothGPU_->SetStructuralShrink(structuralStretch);
	clothGPU_->SetShearStretch(shearShrink);
	clothGPU_->SetShearShrink(shearStretch);
	clothGPU_->SetBendingStretch(bendingShrink);
	clothGPU_->SetBendingShrink(bendingStretch);
	// 速度制限
	clothGPU_->SetVelocityLimit(velocityLimit);
	// 更新回数
	clothGPU_->SetRelaxation(relaxation);

	// 登録
	clothGPU_->CollisionDataRegistration(plane_->GetName(), ClothGPUCollision::kCollisionTypeIndexPlane);
	plane_->SetExsit(true);
	// 登録
	clothGPU_->CollisionDataRegistration(sphere_->GetName(), ClothGPUCollision::kCollisionTypeIndexSphere);
	sphere_->SetExsit(true);
	// 登録
	clothGPU_->CollisionDataRegistration(capsule_->GetName(), ClothGPUCollision::kCollisionTypeIndexCapsule);
	capsule_->SetExsit(true);
}

void ClothDemo::ClothFixedEnd()
{

	// 左上
	clothGPU_->SetWeight(0, 0, false);
	clothGPU_->SetPosition(0, 0, 
		{ resetPosition_.x  - clothScale_.x / 2.0f, resetPosition_.y, resetPosition_.z - clothScale_.y / 2.0f });
	// 右上
	clothGPU_->SetWeight(0, static_cast<uint32_t>(clothDiv_.x), false);
	clothGPU_->SetPosition(0, static_cast<uint32_t>(clothDiv_.x), 
		{ resetPosition_.x + clothScale_.x / 2.0f, resetPosition_.y, resetPosition_.z - clothScale_.y / 2.0f });
	// 左下
	clothGPU_->SetWeight(static_cast<uint32_t>(clothDiv_.y), 0, false);
	clothGPU_->SetPosition(static_cast<uint32_t>(clothDiv_.y), 0, 
		{ resetPosition_.x - clothScale_.x / 2.0f, resetPosition_.y, resetPosition_.z + clothScale_.y / 2.0f });
	// 右下
	clothGPU_->SetWeight(static_cast<uint32_t>(clothDiv_.y), static_cast<uint32_t>(clothDiv_.x), false);
	clothGPU_->SetPosition(static_cast<uint32_t>(clothDiv_.y), static_cast<uint32_t>(clothDiv_.x),
		{ resetPosition_.x + clothScale_.x / 2.0f, resetPosition_.y, resetPosition_.z + clothScale_.y / 2.0f });

}

void ClothDemo::ClothFixedTop()
{

	for (uint32_t x = 0; x <= static_cast<uint32_t>(clothDiv_.x); ++x) {
		clothGPU_->SetWeight(0, x, false);
		clothGPU_->SetPosition(0, x, 
			{ resetPosition_.x - (clothScale_.x / 2.0f) + (clothScale_.x * (static_cast<float>(x) / clothDiv_.x)),
			resetPosition_.y, resetPosition_.z});
	}

}

void ClothDemo::PlaneSwitching()
{

	if (plane_->GetExist()) {
		// 削除
		clothGPU_->CollisionDataDelete(plane_->GetName());
		plane_->SetExsit(false);
	}
	else {
		// 登録
		clothGPU_->CollisionDataRegistration(plane_->GetName(), ClothGPUCollision::kCollisionTypeIndexPlane);
		plane_->SetExsit(true);
	}

}

void ClothDemo::SphereSwitching()
{

	if (sphere_->GetExist()) {
		// 削除
		clothGPU_->CollisionDataDelete(sphere_->GetName());
		sphere_->SetExsit(false);
	}
	else {
		// 登録
		clothGPU_->CollisionDataRegistration(sphere_->GetName(), ClothGPUCollision::kCollisionTypeIndexSphere);
		sphere_->SetExsit(true);
	}

}

void ClothDemo::CapsuleSwitching()
{

	if (capsule_->GetExist()) {
		// 削除
		clothGPU_->CollisionDataDelete(capsule_->GetName());
		capsule_->SetExsit(false);
	}
	else {
		// 登録
		clothGPU_->CollisionDataRegistration(capsule_->GetName(), ClothGPUCollision::kCollisionTypeIndexCapsule);
		capsule_->SetExsit(true);
	}

}
