#include "DebugCamera.h"
#include "../Input/Input.h"
#include "../2D/ImguiManager.h"

/// <summary>
/// 初期化
/// </summary>
void DebugCamera::Initialize() {

	BaseCamera::Initialize();

	moveSpeed_ = 0.1f;
	rotateSpeed_ = 0.01f;

}

/// <summary>
/// 更新
/// </summary>
void DebugCamera::Update(float elapsedTime) {

	Input* input = Input::GetInstance();
	
	Vector3 velocity = {0.0f,0.0f,0.0f};

	if (input->PushKey(DIK_D)) {
		velocity.x += moveSpeed_;
	}
	if (input->PushKey(DIK_A)) {
		velocity.x -= moveSpeed_;
	}
	if (input->PushKey(DIK_W)) {
		velocity.y += moveSpeed_;
	}
	if (input->PushKey(DIK_S)) {
		velocity.y -= moveSpeed_;
	}
	if (input->PushKey(DIK_E)) {
		velocity.z += moveSpeed_;
	}
	if (input->PushKey(DIK_Q)) {
		velocity.z -= moveSpeed_;
	}

	velocity = Matrix4x4::TransformNormal(velocity, transformMatrix_);

	transform_.translate += velocity;

	if (input->PushKey(DIK_RIGHT)) {
		transform_.rotate.y += rotateSpeed_;
	}
	if (input->PushKey(DIK_LEFT)) {
		transform_.rotate.y -= rotateSpeed_;
	}
	if (input->PushKey(DIK_DOWN)) {
		transform_.rotate.x += rotateSpeed_;
	}
	if (input->PushKey(DIK_UP)) {
		transform_.rotate.x -= rotateSpeed_;
	}

	BaseCamera::Update(elapsedTime);

}

void DebugCamera::ImGuiDraw()
{

	ImGui::Begin("DebugCamera");
	ImGui::DragFloat3("translate",&transform_.translate.x,0.01f);
	ImGui::DragFloat3("rotate", &transform_.rotate.x, 0.01f);
	ImGui::End();

}
