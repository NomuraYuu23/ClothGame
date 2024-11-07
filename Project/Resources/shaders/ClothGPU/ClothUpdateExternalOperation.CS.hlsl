#include "ClothGPU.CS.hlsli"

ConstantBuffer<Nums> gNums : register(b0);

RWStructuredBuffer<ClothMassPoint> gClothMassPoints : register(u0);

StructuredBuffer<ExternalOperationData> gExternalOperationDatas : register(t1);

[numthreads(1024, 1, 1)]
void main(uint32_t3 dispatchId : SV_DispatchThreadID)
{

	uint32_t index = dispatchId.x;

	if (gNums.massPointNum_ > index) {

		gClothMassPoints[index].weight_ = gExternalOperationDatas[index].weight_;

		if (gExternalOperationDatas[index].isMove_ == 1) {
			gClothMassPoints[index].position_ = gExternalOperationDatas[index].position_;
			gClothMassPoints[index].prePosition_ = gExternalOperationDatas[index].position_;
		}
	
	}
}