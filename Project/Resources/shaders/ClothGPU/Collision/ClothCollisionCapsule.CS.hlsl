#include "../ClothGPU.CS.hlsli"

struct Capsule {
	// 半径
	float32_t radius_;
	// 原点
	float32_t3 origin_;
	// 終点までのベクトル
	float32_t3 diff_;
};

RWStructuredBuffer<ClothMassPoint> gClothMassPoints : register(u0);

ConstantBuffer<Nums> gNums : register(b0);

ConstantBuffer<Capsule> gCapsule : register(b1);

[numthreads(1024, 1, 1)]
void main(uint32_t3 dispatchId : SV_DispatchThreadID)
{

	uint32_t index = dispatchId.x;

	if (gNums.massPointNum_ > index) {

		ClothMassPoint massPoint = gClothMassPoints[index];

		// 固定点なら終了
		if (massPoint.weight_ == 0.0f) {
			return;
		}

		// カプセルと質点の最も近い点
		float32_t t = dot(massPoint.position_ - gCapsule.origin_, gCapsule.diff_) * rcp(pow(length(gCapsule.diff_), 2.0f));
		t = clamp(t, 0.0f, 1.0f);
		float32_t3 closestPoint = gCapsule.origin_ + gCapsule.diff_* t;

		// 最も近い点から質点に向かうベクトル
		float32_t3 subVector = massPoint.position_ - closestPoint;
		// 距離
		float32_t distance = length(subVector);

		if (distance < gCapsule.radius_) {
			// 衝突したので押し出し
			subVector = subVector * gCapsule.radius_ * rcp(distance);
			gClothMassPoints[index].position_ = closestPoint + subVector;
		}

	}

}