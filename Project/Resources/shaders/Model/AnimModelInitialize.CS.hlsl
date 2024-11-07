struct Vertex {
	float32_t4 position;
	float32_t2 texcoord;
	float32_t3 normal;
};

struct SkinningInformation {
	uint32_t num;
	bool isInverse;
};

// 頂点数
ConstantBuffer<SkinningInformation> gSkinningInformation : register(b0);

RWStructuredBuffer<Vertex> gVertices : register(u0);

[numthreads(1024, 1, 1)]
void main(uint32_t3 DTid : SV_DispatchThreadID)
{

	uint32_t vertexIndex = DTid.x;

	if (vertexIndex < gSkinningInformation.num) {
		gVertices[vertexIndex] = (Vertex)0;
	}

}