#include "ClothGPU.CS.hlsli"
#include "../RandomGenerator/RandomGenerator.hlsli"

ConstantBuffer<Nums> gNums : register(b0);

ConstantBuffer<PerFrame> gPerFrame : register(b1);

ConstantBuffer<ClothCalcData> gClothCalcData : register(b2);

RWStructuredBuffer<ClothMassPoint> gClothMassPoints : register(u0);

StructuredBuffer<ClothSpring> gClothSprings : register(t1);

RWStructuredBuffer<uint32_t> gSpringIndex :  register(u2);

[numthreads(1024, 1, 1)]
void main(uint32_t3 dispatchId : SV_DispatchThreadID)
{

	uint32_t index = dispatchId.x;

	if (index == 0) {
		gSpringIndex[0]++;
		if (gSpringIndex[0] >= 12) {
			gSpringIndex[0] = 0;
		}
	}

	// ここでたいき
	AllMemoryBarrierWithGroupSync();

	uint32_t num = 0;

	if (gSpringIndex[0] < 4) {
		num = gNums.structuralSpringNums_[gSpringIndex[0] % 4];
	}
	else if (gSpringIndex[0] < 8) {
		num = gNums.shearSpringNums_[gSpringIndex[0] % 4];
	}
	else {
		num = gNums.bendingSpringNums_[gSpringIndex[0] % 4];
	}

	if (num > index) {

		ClothSpring spring = gClothSprings[index];

		ClothMassPoint point0 = gClothMassPoints[spring.pointIndex0_];
		ClothMassPoint point1 = gClothMassPoints[spring.pointIndex1_];

		// 二点が固定点
		if (point0.weight_ + point1.weight_ == 0.0f) {
			return;
		}

		// 抵抗
		float32_t shrink = 0.0f;
		float32_t stretch = 0.0f;
		if (spring.type_ == 0) {
			shrink = gClothCalcData.structuralShrink_;
			stretch = gClothCalcData.structuralStretch_;
		}
		else if (spring.type_ == 1) {
			shrink = gClothCalcData.shearShrink_;
			stretch = gClothCalcData.shearStretch_;
		}
		else {
			shrink = gClothCalcData.bendingShrink_;
			stretch = gClothCalcData.bendingStretch_;
		}

		// バネの力
		// 質点間の距離
		float32_t distance = length(point1.position_ - point0.position_);
		// 力
		float32_t force = (distance - spring.naturalLength_) * gClothCalcData.stiffness_;
		// 抵抗
		if (force >= 0.0f) {
			force *= shrink;
		}
		else {
			force *= stretch;
		}

		// 変位
		float32_t3 dx = (float32_t3)0;
		dx = point1.position_ - point0.position_;
		if (!(dx.x == 0.0f &&
			dx.y == 0.0f &&
			dx.z == 0.0f)) {
			dx = normalize(dx);
			dx *= force;
			dx *= gPerFrame.deltaTime * gPerFrame.deltaTime * 0.5f * rcp(gClothCalcData.mass_);
		}

		// 位置更新
		float32_t3 dx0 = (float32_t3)0;
		dx0 = dx * (point0.weight_ / (point0.weight_ + point1.weight_));
		gClothMassPoints[spring.pointIndex0_].position_ += dx0;
		float32_t3 dx1 = (float32_t3)0;
		dx1 = dx * (point1.weight_ / (point0.weight_ + point1.weight_));
		gClothMassPoints[spring.pointIndex1_].position_ -= dx1;
	
	}

}