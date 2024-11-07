#include "GPUParticle.hlsli"

struct VertexShaderInput {
	float32_t4 position : POSITION0;
	float32_t2 texcoord : TEXCOORD0;
};

struct Particle {

	float32_t3 translate;
	float32_t3 scale;
	float32_t lifeTime;
	float32_t3 velocity;
	float32_t currentTime;
	float32_t4 color;
	float32_t3 rotate;
	float32_t3 rotateVelocity;
	float32_t4x4 rotateMatrix;

};

struct ParticleVIew {

	float32_t4x4 viewProjection;
	float32_t4x4 billboardMatrix;

};

StructuredBuffer<Particle> gParticles : register(t4);

ConstantBuffer<ParticleVIew> gParticleVIew : register(b6);

VertexShaderOutput main(VertexShaderInput input, uint32_t instanceId : SV_InstanceId)
{

	VertexShaderOutput output;
	Particle particle = gParticles[instanceId];
	float32_t4x4 worldMatrix = particle.rotateMatrix;
	worldMatrix[0] *= particle.scale.x;
	worldMatrix[1] *= particle.scale.y;
	worldMatrix[2] *= particle.scale.z;
	worldMatrix[3].xyz = particle.translate;
	output.position = mul(input.position, mul(worldMatrix, gParticleVIew.viewProjection));
	output.texcoord = input.texcoord;
	output.color = particle.color;

	return output;

}