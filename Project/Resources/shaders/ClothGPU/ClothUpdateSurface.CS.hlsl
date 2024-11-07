#include "ClothGPU.CS.hlsli"

RWStructuredBuffer<SurfaceData> gSurfaceDatas : register(u0);

ConstantBuffer<Nums> gNums : register(b0);

RWStructuredBuffer<ClothMassPoint> gClothMassPoints : register(u1);

[numthreads(1024, 1, 1)]
void main(uint32_t3 dispatchId : SV_DispatchThreadID)
{

	uint32_t index = dispatchId.x;

	if (gNums.surfaceNum_ > index) {

		// 左上
		float32_t3 leftTop = gClothMassPoints[gSurfaceDatas[index].indexes_[0]].position_;
		// 右上
		float32_t3 rightTop = gClothMassPoints[gSurfaceDatas[index].indexes_[1]].position_;
		// 左下
		float32_t3 leftBottom = gClothMassPoints[gSurfaceDatas[index].indexes_[2]].position_;
		// 右下
		float32_t3 rightBottom = gClothMassPoints[gSurfaceDatas[index].indexes_[3]].position_;

		// 法線作成

		// 1つ目の三角形のクロス積を計算する
		float32_t3 cross0 = cross(rightBottom - rightTop, leftBottom - rightTop);
		// 2つ目の三角形
		float32_t3 cross1 = cross(leftBottom - rightTop, leftTop - rightTop);
		// 2つのクロス積の結果を合わせる
		float32_t3 normal = cross0 + cross1;
		// 単位ベクトルへ正則化
		normal = normalize(normal);
		gSurfaceDatas[index].normal_ = normal;

	}

}