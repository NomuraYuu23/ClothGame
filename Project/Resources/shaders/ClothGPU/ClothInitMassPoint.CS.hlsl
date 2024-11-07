#include "ClothGPU.CS.hlsli"

RWStructuredBuffer<ClothMassPoint> gClothMassPoints : register(u0);

ConstantBuffer<Nums> gNums : register(b0);

ConstantBuffer<CreateData> gCreateData : register(b1);

RWStructuredBuffer<uint32_t> gSpringIndex :  register(u1);

[numthreads(1024, 1, 1)]
void main(uint32_t3 dispatchId : SV_DispatchThreadID)
{

	uint32_t index = dispatchId.x;

	if (index == 0) {
		gSpringIndex[0] = 11;
	}

	if (gNums.massPointNum_ > index) {

		float32_t y = index / uint32_t(gCreateData.div_.x + 1);
		float32_t x = index % uint32_t(gCreateData.div_.x + 1);

		gClothMassPoints[index].position_ = 
			float32_t3(
				x / gCreateData.div_.x,
				y / gCreateData.div_.y * -1.0f,
					0.0f);

		gClothMassPoints[index].position_.x *= gCreateData.scale_.x;
		gClothMassPoints[index].position_.y *= gCreateData.scale_.y;

		gClothMassPoints[index].prePosition_ = gClothMassPoints[index].position_;

		if (y == 0) {
			gClothMassPoints[index].weight_ = 0.0f;
		}
		else {
			gClothMassPoints[index].weight_ = 1.0f;
		}

	}

}