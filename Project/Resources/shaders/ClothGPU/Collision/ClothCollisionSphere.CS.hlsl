#include "../ClothGPU.CS.hlsli"

struct Sphere {
	// 位置
	float32_t3 position_;
	// 距離
	float32_t radius_;
};

RWStructuredBuffer<ClothMassPoint> gClothMassPoints : register(u0);

ConstantBuffer<Nums> gNums : register(b0);

ConstantBuffer<Sphere> gSphere : register(b1);

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

		float32_t3 subVector = massPoint.position_ - gSphere.position_;

		float32_t distance = length(subVector);

		if (distance < gSphere.radius_) {
			// 衝突したので押し出し
			subVector = subVector * gSphere.radius_ * rcp(distance);
			
			float32_t3 newPosition = gSphere.position_ + subVector;

			gClothMassPoints[index].position_ = newPosition;
			gClothMassPoints[index].prePosition_ = newPosition;

		}

	}

}