#include "ClothGPU.CS.hlsli"

RWStructuredBuffer<VertexData> gVertexDatas : register(u0);

ConstantBuffer<Nums> gNums : register(b0);

StructuredBuffer<uint32_t> gMassPointIndexes : register(t1);

RWStructuredBuffer<ClothMassPoint> gClothMassPoints : register(u2);

RWStructuredBuffer<SurfaceData> gSurfaceDatas : register(u3);

[numthreads(1024, 1, 1)]
void main(uint32_t3 dispatchId : SV_DispatchThreadID)
{

	uint32_t index = dispatchId.x;

	if (gNums.vertexNum_ > index) {

		// 頂点の持つ質点の番号取得
		uint32_t massPointIndex = gMassPointIndexes[index];

		// 位置設定
		float32_t3 position = gClothMassPoints[massPointIndex].position_;

		gVertexDatas[index].position_ =
			float32_t4(
				position.x,
				position.y,
				position.z,
				1.0f);

		// 法線マッピング
		float32_t3 normals[4];
		uint32_t normalNum = 0;

		uint32_t j = 0;
		uint32_t k = 0;

		// 最大4つの法線情報を取得
		for (j = 0; j < gNums.surfaceNum_; ++j) {
			for (k = 0; k < 4; ++k) {
				if (gSurfaceDatas[j].indexes_[k] == massPointIndex) {
					normals[normalNum] = gSurfaceDatas[j].normal_;
					normalNum++;
					break;
				}
			}

			if (normalNum >= 4) {
				break;
			}

		}

		float32_t3 normal = { 0.0f,0.0f,0.0f };
		for (j = 0; j < normalNum; ++j) {
			normal += normals[j];
		}

		// 法線代入
		gVertexDatas[index].normal_ = normalize(normal);

	}

}
