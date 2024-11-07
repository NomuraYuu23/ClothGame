#pragma once
#include "../../Math/Vector/Vector4.h"
#include "../../Math/Matrix/Matrix4x4.h"
#include <cstdint>

enum EnableLighting {

	None = 0,
	Lambert = 1,
	HalfLambert = 2,
	PhongReflection = 3,
	BlinnPhongReflection = 4,

};

struct MaterialData {
	Vector4 color;
	int32_t enableLighting;
	float padding[3];
	Matrix4x4 uvTransform;
	float shininess;
	float environmentCoefficient;
};

struct SRVMaterialData {
	Vector4 color;
	int32_t enableLighting;
	Matrix4x4 uvTransform;
	float shininess;
	float environmentCoefficient;
};
