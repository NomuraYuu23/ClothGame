#include "../Cloth/Cloth.hlsli"

struct TransformationMatrix {
	float32_t4x4 WVP;
};

ConstantBuffer<TransformationMatrix> gTransformationMatrix : register(b5);

struct Vertex {
	float32_t4 position;
	float32_t2 texcoord;
	float32_t3 normal;
};

StructuredBuffer<Vertex> gVertices : register(t5);

VertexShaderOutput main(uint32_t vertID : SV_VertexID) {

	VertexShaderOutput output;

	output.position = mul(gVertices[vertID].position, gTransformationMatrix.WVP);
	output.texcoord = gVertices[vertID].texcoord;
	output.normal = gVertices[vertID].normal;
	output.worldPosition = gVertices[vertID].position.xyz;

	return output;

}
