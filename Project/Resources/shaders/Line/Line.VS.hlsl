#include "Line.hlsli"

static const uint32_t kNumVertex = 2;

struct ViewProjectionMatrix {
	float32_t4x4 m;
};

ConstantBuffer<ViewProjectionMatrix> gViewProjectionMatrix : register(b3);

struct LineGPU {

	float32_t3 positions[kNumVertex];
	float32_t4 colors[kNumVertex];

};

StructuredBuffer<LineGPU> gLine : register(t3);

VertexShaderOutput main(uint32_t vertexId : SV_VertexID, uint32_t instanceId : SV_InstanceID)
{

	VertexShaderOutput output;
	
	float32_t4 inputPosition = float32_t4(gLine[instanceId].positions[vertexId], 1.0f);

	output.position = mul(inputPosition, gViewProjectionMatrix.m);
	output.color = gLine[instanceId].colors[vertexId];

	return output;

}