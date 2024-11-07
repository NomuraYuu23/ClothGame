#include "ClothGPU.CS.hlsli"

RWStructuredBuffer<VertexData> gVertexDatas : register(u0);

ConstantBuffer<Nums> gNums : register(b0);

ConstantBuffer<CreateData> gCreateData : register(b1);

StructuredBuffer<uint32_t> gMassPointIndexes : register(t1);

[numthreads(1024, 1, 1)]
void main( uint32_t3 dispatchId : SV_DispatchThreadID )
{

	uint32_t index = dispatchId.x;

	if (gNums.vertexNum_ > index) {

		uint32_t massPointIndex = gMassPointIndexes[index];

		uint32_t xMax = uint32_t(gCreateData.div_.x + 1.0f);

		uint32_t y = massPointIndex / xMax;
		uint32_t x = massPointIndex % xMax;

		gVertexDatas[index].position_ = 
			float32_t4(
				x / gCreateData.div_.x,
				y / gCreateData.div_.y * -1.0f,
				0.0f,
				1.0f);

		gVertexDatas[index].position_.x *= gCreateData.scale_.x;
		gVertexDatas[index].position_.y *= gCreateData.scale_.y;

		gVertexDatas[index].texcoord_ = float32_t2(float32_t(x) * rcp(gCreateData.div_.x), float32_t(y) * rcp(gCreateData.div_.y));
		gVertexDatas[index].normal_ = float32_t3(0.0f, 0.0f, -1.0f);
	
	}

}