#include "ClothGPU.CS.hlsli"

RWStructuredBuffer<SurfaceData> gSurfaceDatas : register(u0);

ConstantBuffer<Nums> gNums : register(b0);

StructuredBuffer<uint32_t> gMassPointIndexes : register(t1);

[numthreads(1024, 1, 1)]
void main(uint32_t3 dispatchId : SV_DispatchThreadID)
{

	uint32_t index = dispatchId.x;

	if (gNums.surfaceNum_ > index) {

		uint32_t vertexIndexOffset = index * 6;

		gSurfaceDatas[index].indexes_.x = gMassPointIndexes[vertexIndexOffset];
		gSurfaceDatas[index].indexes_.y = gMassPointIndexes[vertexIndexOffset + 1];
		gSurfaceDatas[index].indexes_.z = gMassPointIndexes[vertexIndexOffset + 2];
		gSurfaceDatas[index].indexes_.w = gMassPointIndexes[vertexIndexOffset + 5];

		gSurfaceDatas[index].normal_ = float32_t3(0.0f, 0.0f, -1.0f);

	}

}