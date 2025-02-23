#include "Model.hlsli"

struct TransformationMatrix {
	float32_t4x4 WVP;
	float32_t4x4 World;
	float32_t4x4 WorldInverseTranspose;
};

ConstantBuffer<TransformationMatrix> gTransformationMatrix : register(b0);

struct Vertex {
	float32_t4 position;
	float32_t2 texcoord;
	float32_t3 normal;
};

StructuredBuffer<Vertex> gVertices : register(t5);

StructuredBuffer<uint32_t> gVertexIndices : register(t6);

VertexShaderOutput main(uint32_t vertID : SV_VertexID) {

	VertexShaderOutput output;
	
	// index
    uint32_t id = gVertexIndices[vertID];
	
	// texcoord
    output.texcoord = gVertices[id].texcoord;

    output.position = mul(gVertices[id].position, gTransformationMatrix.WVP);

    output.normal = normalize(mul(gVertices[id].normal, (float32_t3x3) gTransformationMatrix.WorldInverseTranspose));

    output.worldPosition = mul(gVertices[id].position, gTransformationMatrix.World).xyz;

	return output;
}