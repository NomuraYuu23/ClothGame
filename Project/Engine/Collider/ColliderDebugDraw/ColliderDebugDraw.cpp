#include "ColliderDebugDraw.h"
#include "../../2D/ImguiManager.h"
#include <numbers>
#include <cmath>

void ColliderDebugDraw::Initialize()
{

	ListClear();

	isDraw_ = false;

	InitializeOBB();

	InitializeSphere();

}

void ColliderDebugDraw::ListClear()
{

	colliders_.clear();

}


void ColliderDebugDraw::AddCollider(const ColliderShape& collider)
{

	colliders_.push_back(collider);

}

void ColliderDebugDraw::DrawMap(DrawLine* drawLine)
{

	if (!isDraw_) {
		return;
	}

	for (std::list<ColliderShape>::iterator itr = colliders_.begin();
		itr != colliders_.end(); ++itr) {

		ColliderShape collider = *itr;

		// OBBなら
		if (std::holds_alternative<OBB>(collider)) {

			OBB obb = std::get<OBB>(collider);
			DrawMapOBB(drawLine, obb);

		}
		else if (std::holds_alternative<Sphere>(collider)) {
			Sphere sphere = std::get<Sphere>(collider);
			DrawMapSphere(drawLine, sphere);
		}

	}

}

void ColliderDebugDraw::ImGuiDraw()
{
#ifdef _DEMO
	ImGui::Begin("ColliderDebugDraw");
	ImGui::Checkbox("描画するか", &isDraw_);
	ImGui::End();
#endif // _DEMO
}

void ColliderDebugDraw::InitializeOBB()
{

	obbOffsetPoints_ = {
		{
			{-1.0f, -1.0f, -1.0f},
			{+1.0f, -1.0f, -1.0f},
			{-1.0f, +1.0f, -1.0f},
			{+1.0f, +1.0f, -1.0f},
			{-1.0f, -1.0f, +1.0f},
			{+1.0f, -1.0f, +1.0f},
			{-1.0f, +1.0f, +1.0f},
			{+1.0f, +1.0f, +1.0f}
		}
	};

}

void ColliderDebugDraw::DrawMapOBB(DrawLine* drawLine, const OBB& collider)
{
	
	std::array<Vector3, 8> points = {};
	
	Vector3 size = collider.size_;
	Matrix4x4 rotateMatrix = Matrix4x4::MakeIdentity4x4();

	rotateMatrix.m[0][0] = collider.otientatuons_[0].x;
	rotateMatrix.m[0][1] = collider.otientatuons_[0].y;
	rotateMatrix.m[0][2] = collider.otientatuons_[0].z;

	rotateMatrix.m[1][0] = collider.otientatuons_[1].x;
	rotateMatrix.m[1][1] = collider.otientatuons_[1].y;
	rotateMatrix.m[1][2] = collider.otientatuons_[1].z;

	rotateMatrix.m[2][0] = collider.otientatuons_[2].x;
	rotateMatrix.m[2][1] = collider.otientatuons_[2].y;
	rotateMatrix.m[2][2] = collider.otientatuons_[2].z;

	Vector3 center = collider.center_;

	for (uint32_t i = 0; i < obbOffsetPoints_.size(); ++i) {
		
		// offset * size
		points[i].x = obbOffsetPoints_[i].x * size.x;
		points[i].y = obbOffsetPoints_[i].y * size.y;
		points[i].z = obbOffsetPoints_[i].z * size.z;

		// 軸で回転
		points[i] = Matrix4x4::TransformNormal(points[i], rotateMatrix);

		// centerに移動
		points[i] += center;

	}

	LineForGPU lineForGPU = {};
	lineForGPU.color[0] = { 1.0f,1.0f,1.0f,1.0f };
	lineForGPU.color[1] = { 1.0f,1.0f,1.0f,1.0f };

	lineForGPU.position[0] = points[0];
	lineForGPU.position[1] = points[1];
	drawLine->Map(lineForGPU);
	lineForGPU.position[0] = points[2];
	lineForGPU.position[1] = points[3];
	drawLine->Map(lineForGPU);
	lineForGPU.position[0] = points[0];
	lineForGPU.position[1] = points[2];
	drawLine->Map(lineForGPU);
	lineForGPU.position[0] = points[1];
	lineForGPU.position[1] = points[3];
	drawLine->Map(lineForGPU);

	lineForGPU.position[0] = points[4];
	lineForGPU.position[1] = points[5];
	drawLine->Map(lineForGPU);
	lineForGPU.position[0] = points[6];
	lineForGPU.position[1] = points[7];
	drawLine->Map(lineForGPU);
	lineForGPU.position[0] = points[4];
	lineForGPU.position[1] = points[6];
	drawLine->Map(lineForGPU);
	lineForGPU.position[0] = points[5];
	lineForGPU.position[1] = points[7];
	drawLine->Map(lineForGPU);

	lineForGPU.position[0] = points[0];
	lineForGPU.position[1] = points[4];
	drawLine->Map(lineForGPU);
	lineForGPU.position[0] = points[1];
	lineForGPU.position[1] = points[5];
	drawLine->Map(lineForGPU);
	lineForGPU.position[0] = points[2];
	lineForGPU.position[1] = points[6];
	drawLine->Map(lineForGPU);
	lineForGPU.position[0] = points[3];
	lineForGPU.position[1] = points[7];
	drawLine->Map(lineForGPU);

}

void ColliderDebugDraw::InitializeSphere()
{

	const float kPi = static_cast<float>(std::numbers::pi);

	// 経度分割1つ分の角度
	float kLonEvery = 2.0f * kPi / static_cast<float>(kSubdivision_);
	// 緯度分割1つ分の角度
	float kLatEvery = kPi / static_cast<float>(kSubdivision_);

	// 緯度の方向に分割
	for (uint32_t latIndex = 0; latIndex < kSubdivision_; ++latIndex) {

		// 現在の緯度
		float lat = -1.0f * kPi / 2.0f + kLatEvery * static_cast<float>(latIndex);

		// 経度の方向に分割
		for (uint32_t lonIndex = 0; lonIndex < kSubdivision_; ++lonIndex) {
		
			// 現在の経度
			float lon = static_cast<float>(lonIndex) * kLonEvery;

			sphereOffsetPoints_[latIndex * kSubdivision_ + lonIndex] = {
				std::cosf(lat) * std::cosf(lon),
				std::sinf(lat),
				std::cosf(lat)* std::sinf(lon)
			};

		}

	}

	// ラスト一個
	sphereOffsetPoints_[kSubdivision_ * kSubdivision_] = { 0.0f, std::sinf(kPi / 2.0f), 0.0f };

}

void ColliderDebugDraw::DrawMapSphere(DrawLine* drawLine, const Sphere& collider)
{

	std::array<Vector3, kSubdivision_ * kSubdivision_ + 1> points = {};

	Vector3 center = collider.center_;
	float raidus = collider.radius_;

	for (uint32_t i = 0; i < points.size(); ++i) {

		// offset * size
		points[i] = sphereOffsetPoints_[i] * raidus;

		// centerに移動
		points[i] += center;

	}

	LineForGPU lineForGPU = {};
	lineForGPU.color[0] = { 1.0f,1.0f,1.0f,1.0f };
	lineForGPU.color[1] = { 1.0f,1.0f,1.0f,1.0f };

	for (uint32_t i = 0; i < kSubdivision_ * kSubdivision_; ++i) {

		// kSubdivisionの倍数なら
		if (i % kSubdivision_ == 0) {
			lineForGPU.position[0] = points[i];
			lineForGPU.position[1] = points[i + kSubdivision_ - 1];
		}
		else {
			lineForGPU.position[0] = points[i];
			lineForGPU.position[1] = points[i - 1];
		}

		drawLine->Map(lineForGPU);

		if (i >= kSubdivision_ * (kSubdivision_ - 1)) {
			lineForGPU.position[0] = points[i];
			lineForGPU.position[1] = points[kSubdivision_ * kSubdivision_];
		}
		else {
			lineForGPU.position[0] = points[i];
			lineForGPU.position[1] = points[i + kSubdivision_];
		}

		drawLine->Map(lineForGPU);

	}

}
