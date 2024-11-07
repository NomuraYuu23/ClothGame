#include "ManyModels.hlsli"

struct Material {
	float32_t4 color;
	int32_t enableLighting;
	float32_t4x4 uvTransform;
	float32_t shininess;
	float32_t environmentCoefficient;
};

struct DirectionalLight {
	float32_t4 color;
	float32_t3 direction;
	float intencity;
};

struct Camera {
	float32_t3 worldPosition;
};

struct PointLight {
	float32_t4 color; // ライト色
	float32_t3 position; // ライトの位置
	float intencity; // 輝度
	float radius; // ライトの届く最大距離
	float decay; // 減衰率
	bool used; // 使用している
};

struct SpotLight {
	float32_t4 color; // ライト色
	float32_t3 position; // ライトの位置
	float32_t intencity; // 輝度
	float32_t3 direction; // スポットライトの方向
	float32_t distance; // ライトの届く最大距離
	float32_t decay; // 減衰率
	float32_t cosAngle; // スポットライトの余弦
	float32_t cosFalloffStart; // フォールオフ開始位置
	bool used; // 使用している
};

struct Fog {
	float32_t4 color; // 色
	float32_t fagNear; // 開始位置
	float32_t fagFar; // 終了位置
};

struct PointLightCalcData {
	float32_t3 pointLightDirection;
	float32_t pointFactor;
	bool used;
};

struct SpotLightCalcData {
	float32_t3 spotLightDirectionOnSuface;
	float32_t spotFactor;
	bool used;
};

struct PixelShaderOutput {
	float32_t4 color : SV_TARGET0;
	float32_t4 normal : SV_TARGET1;
};

SamplerState gSampler : register(s0);

Texture2D<float32_t4> gTexture0 : register(t0);
Texture2D<float32_t4> gTexture1 : register(t1);
Texture2D<float32_t4> gTexture2 : register(t2);
Texture2D<float32_t4> gTexture3 : register(t3);
Texture2D<float32_t4> gTexture4 : register(t4);
Texture2D<float32_t4> gTexture5 : register(t5);
Texture2D<float32_t4> gTexture6 : register(t6);
Texture2D<float32_t4> gTexture7 : register(t7);

StructuredBuffer<PointLight> gPointLights : register(t8);
StructuredBuffer<SpotLight> gSpotLights : register(t9);
StructuredBuffer<Material> gMaterials : register(t10);
TextureCube<float32_t4> gEnvironmentTexture : register(t11);

ConstantBuffer<DirectionalLight> gDirectionalLight : register(b1);

ConstantBuffer<Camera> gCamera : register(b2);

ConstantBuffer<Fog> gFog : register(b3);

/// <summary>
/// ランバート
/// </summary>
float32_t4 Lambert(VertexShaderOutput input, float32_t4 textureColor,
	PointLightCalcData pointLightCalcDatas[4],
	SpotLightCalcData spotLightCalcDatas[4],
	uint32_t instanceID) {

	float32_t4 color;

	// 平行光源
	float directionalLightCos = saturate(dot(normalize(input.normal), -gDirectionalLight.direction));
	float32_t3 directionalLightColor = gDirectionalLight.color.rgb * directionalLightCos * gDirectionalLight.intencity;

	// ポイントライト
	float32_t3 allPointLightColor = { 0.0,0.0,0.0 };
	for (int i = 0; i < 4; i++) {
		if (pointLightCalcDatas[i].used) {
			float pointLightCos = saturate(dot(normalize(input.normal), -pointLightCalcDatas[i].pointLightDirection));
			float32_t3 pointLightColor = gPointLights[i].color.rgb * pointLightCos * gPointLights[i].intencity * pointLightCalcDatas[i].pointFactor;
			allPointLightColor += pointLightColor;
		}
	}

	// スポットライト
	float32_t3 allSpotLightColor = { 0.0,0.0,0.0 };
	for (int j = 0; j < 4; j++) {
		if (spotLightCalcDatas[j].used) {
			float spotLightCos = saturate(dot(normalize(input.normal), -spotLightCalcDatas[j].spotLightDirectionOnSuface));
			float32_t3 spotLightColor = gSpotLights[j].color.rgb * spotLightCos * gSpotLights[j].intencity * spotLightCalcDatas[j].spotFactor;
			allSpotLightColor += spotLightColor;
		}
	}

	// 全てのライトデータを入れる
	color.rgb = gMaterials[instanceID].color.rgb * textureColor.rgb * (directionalLightColor + allPointLightColor + allSpotLightColor);
	color.a = gMaterials[instanceID].color.a * textureColor.a;

	return color;

}

/// <summary>
/// 半ランバート
/// </summary>
float32_t4 HalfLambert(VertexShaderOutput input, float32_t4 textureColor,
	PointLightCalcData pointLightCalcDatas[4],
	SpotLightCalcData spotLightCalcDatas[4],
	uint32_t instanceID) {

	float32_t4 color;

	// 平行光源
	float directionalLightNdotL = dot(normalize(input.normal), -gDirectionalLight.direction);
	float directionalLightCos = pow(directionalLightNdotL * 0.5f + 0.5f, 2.0f);
	float32_t3 directionalLightColor = gDirectionalLight.color.rgb * directionalLightCos * gDirectionalLight.intencity;

	// ポイントライト
	float32_t3 allPointLightColor = { 0.0,0.0,0.0 };
	for (int i = 0; i < 4; i++) {
		if (pointLightCalcDatas[i].used) {
			float pointLightNdotL = dot(normalize(input.normal), -pointLightCalcDatas[i].pointLightDirection);
			float pointLightCos = pow(pointLightNdotL * 0.5f + 0.5f, 2.0f);
			float32_t3 pointLightColor = gPointLights[i].color.rgb * pointLightCos * gPointLights[i].intencity * pointLightCalcDatas[i].pointFactor;
			allPointLightColor += pointLightColor;
		}
	}

	// スポットライト
	float32_t3 allSpotLightColor = { 0.0,0.0,0.0 };
	for (int j = 0; j < 4; j++) {
		if (spotLightCalcDatas[j].used) {
			float spotLightNdotL = dot(normalize(input.normal), -spotLightCalcDatas[j].spotLightDirectionOnSuface);
			float spotLightCos = pow(spotLightNdotL * 0.5f + 0.5f, 2.0f);
			float32_t3 spotLightColor = gSpotLights[j].color.rgb * spotLightCos * gSpotLights[j].intencity * spotLightCalcDatas[j].spotFactor;
			allSpotLightColor += spotLightColor;
		}
	}

	// 全てのライトデータを入れる
	color.rgb = gMaterials[instanceID].color.rgb * textureColor.rgb * (directionalLightColor + allPointLightColor + allSpotLightColor);
	color.a = gMaterials[instanceID].color.a * textureColor.a;

	return color;

}

/// <summary>
/// 鏡面反射
/// </summary>
float32_t4 PhongReflection(VertexShaderOutput input, float32_t4 textureColor, float32_t3 toEye,
	PointLightCalcData pointLightCalcDatas[4],
	SpotLightCalcData spotLightCalcDatas[4],
	uint32_t instanceID) {

	float32_t4 color;

	// 平行光源
	float directionalLightNdotL = dot(normalize(input.normal), -gDirectionalLight.direction);
	float directionalLightCos = pow(directionalLightNdotL * 0.5f + 0.5f, 2.0f);
	float32_t3 directionalLightReflectLight = reflect(gDirectionalLight.direction, normalize(input.normal));
	float directionalLightRdotE = dot(directionalLightReflectLight, toEye);
	float directionalLightSpecularPow = pow(saturate(directionalLightRdotE), gMaterials[instanceID].shininess);
	// 拡散反射
	float32_t3 directionalLightDiffuse =
		gMaterials[instanceID].color.rgb * textureColor.rgb * gDirectionalLight.color.rgb * directionalLightCos * gDirectionalLight.intencity;
	// 鏡面反射
	float32_t3 directionalLightSpecular =
		gDirectionalLight.color.rgb * gDirectionalLight.intencity * directionalLightSpecularPow * float32_t3(1.0f, 1.0f, 1.0f);

	// ポイントライト
	// 拡散反射
	float32_t3 allPointLightDiffuse = { 0.0,0.0,0.0 };
	// 鏡面反射
	float32_t3 allPointLightSpecular = { 0.0,0.0,0.0 };
	for (int i = 0; i < 4; i++) {
		if (pointLightCalcDatas[i].used) {
			float pointLightNdotL = dot(normalize(input.normal), -pointLightCalcDatas[i].pointLightDirection);
			float pointLightCos = pow(pointLightNdotL * 0.5f + 0.5f, 2.0f);
			float32_t3 pointLightReflectLight = reflect(pointLightCalcDatas[i].pointLightDirection, normalize(input.normal));
			float pointLightRdotE = dot(pointLightReflectLight, toEye);
			float pointLightSpecularPow = pow(saturate(pointLightRdotE), gMaterials[instanceID].shininess);
			// 拡散反射
			float32_t3 pointLightDiffuse =
				gMaterials[instanceID].color.rgb * textureColor.rgb * gPointLights[i].color.rgb * pointLightCos * gPointLights[i].intencity * pointLightCalcDatas[i].pointFactor;
			allPointLightDiffuse += pointLightDiffuse;
			// 鏡面反射
			float32_t3 pointLightSpecular =
				gPointLights[i].color.rgb * gPointLights[i].intencity * pointLightCalcDatas[i].pointFactor * pointLightSpecularPow * float32_t3(1.0f, 1.0f, 1.0f);
			allPointLightSpecular += pointLightSpecular;
		}
	}


	// スポットライト
	// 拡散反射
	float32_t3 allSpotLightDiffuse = { 0.0,0.0,0.0 };
	// 鏡面反射
	float32_t3 allSpotLightSpecular = { 0.0,0.0,0.0 };
	for (int j = 0; j < 4; j++) {
		if (spotLightCalcDatas[j].used) {
			float spotLightNdotL = dot(normalize(input.normal), -spotLightCalcDatas[j].spotLightDirectionOnSuface);
			float spotLightCos = pow(spotLightNdotL * 0.5f + 0.5f, 2.0f);
			float32_t3 spotLightReflectLight = reflect(spotLightCalcDatas[j].spotLightDirectionOnSuface, normalize(input.normal));
			float spotLightRdotE = dot(spotLightReflectLight, toEye);
			float spotLightSpecularPow = pow(saturate(spotLightRdotE), gMaterials[instanceID].shininess);
			// 拡散反射
			float32_t3 spotLightDiffuse =
				gMaterials[instanceID].color.rgb * textureColor.rgb * gSpotLights[j].color.rgb * spotLightCos * gSpotLights[j].intencity * spotLightCalcDatas[j].spotFactor;
			allSpotLightDiffuse += spotLightDiffuse;
			// 鏡面反射
			float32_t3 spotLightSpecular =
				gSpotLights[j].color.rgb * gSpotLights[j].intencity * spotLightCalcDatas[j].spotFactor * spotLightSpecularPow * float32_t3(1.0f, 1.0f, 1.0f);
			allSpotLightSpecular += spotLightSpecular;
		}
	}

	// 全てのライトデータを入れる
	// 拡散反射+鏡面反射
	color.rgb = directionalLightDiffuse + directionalLightSpecular + allPointLightDiffuse + allPointLightSpecular + allSpotLightDiffuse + allSpotLightSpecular;
	// α
	color.a = gMaterials[instanceID].color.a * textureColor.a;

	return color;

}

/// <summary>
/// ブリン鏡面反射
/// </summary>
float32_t4 BlinnPhongReflection(VertexShaderOutput input, float32_t4 textureColor, float32_t3 toEye,
	PointLightCalcData pointLightCalcDatas[4],
	SpotLightCalcData spotLightCalcDatas[4],
	uint32_t instanceID) {

	float32_t4 color;

	// 平行光源
	float directionalLightNdotL = dot(normalize(input.normal), -gDirectionalLight.direction);
	float directionalLightCos = pow(directionalLightNdotL * 0.5f + 0.5f, 2.0f);
	float32_t3 directionalLightReflectLight = reflect(gDirectionalLight.direction, normalize(input.normal));
	float32_t3 directionalLightHalfVector = normalize(-gDirectionalLight.direction + toEye);
	float directionalLightNDotH = dot(normalize(input.normal), directionalLightHalfVector);
	float directionalLightSpecularPow = pow(saturate(directionalLightNDotH), gMaterials[instanceID].shininess);
	// 拡散反射
	float32_t3 directionalLightDiffuse =
		gMaterials[instanceID].color.rgb * textureColor.rgb * gDirectionalLight.color.rgb * directionalLightCos * gDirectionalLight.intencity;
	// 鏡面反射
	float32_t3 directionalLightSpecular =
		gDirectionalLight.color.rgb * gDirectionalLight.intencity * directionalLightSpecularPow * float32_t3(1.0f, 1.0f, 1.0f);

	// ポイントライト
	// 拡散反射
	float32_t3 allPointLightDiffuse = { 0.0,0.0,0.0 };
	// 鏡面反射
	float32_t3 allPointLightSpecular = { 0.0,0.0,0.0 };
	for (int i = 0; i < 4; i++) {
		if (pointLightCalcDatas[i].used) {
			float pointLightNdotL = dot(normalize(input.normal), -pointLightCalcDatas[i].pointLightDirection);
			float pointLightCos = pow(pointLightNdotL * 0.5f + 0.5f, 2.0f);
			float32_t3 pointLightHalfVector = normalize(-pointLightCalcDatas[i].pointLightDirection + toEye);
			float pointLightNDotH = dot(normalize(input.normal), pointLightHalfVector);
			float pointLightSpecularPow = pow(saturate(pointLightNDotH), gMaterials[instanceID].shininess);
			// 拡散反射
			float32_t3 pointLightDiffuse =
				gMaterials[instanceID].color.rgb * textureColor.rgb * gPointLights[i].color.rgb * pointLightCos * gPointLights[i].intencity * pointLightCalcDatas[i].pointFactor;
			allPointLightDiffuse += pointLightDiffuse;
			// 鏡面反射
			float32_t3 pointLightSpecular =
				gPointLights[i].color.rgb * gPointLights[i].intencity * pointLightCalcDatas[i].pointFactor * pointLightSpecularPow * float32_t3(1.0f, 1.0f, 1.0f);
			allPointLightSpecular += pointLightSpecular;
		}
	}

	// スポットライト
	// 拡散反射
	float32_t3 allSpotLightDiffuse = { 0.0,0.0,0.0 };
	// 鏡面反射
	float32_t3 allSpotLightSpecular = { 0.0,0.0,0.0 };
	for (int j = 0; j < 4; j++) {
		if (spotLightCalcDatas[j].used) {
			float spotLightNdotL = dot(normalize(input.normal), -spotLightCalcDatas[j].spotLightDirectionOnSuface);
			float spotLightCos = pow(spotLightNdotL * 0.5f + 0.5f, 2.0f);
			float32_t3 spotLightHalfVector = normalize(-spotLightCalcDatas[j].spotLightDirectionOnSuface + toEye);
			float spotLightNDotH = dot(normalize(input.normal), spotLightHalfVector);
			float spotLightSpecularPow = pow(saturate(spotLightNDotH), gMaterials[instanceID].shininess);
			// 拡散反射
			float32_t3 spotLightDiffuse =
				gMaterials[instanceID].color.rgb * textureColor.rgb * gSpotLights[j].color.rgb * spotLightCos * gSpotLights[j].intencity * spotLightCalcDatas[j].spotFactor;
			allSpotLightDiffuse += spotLightDiffuse;
			// 鏡面反射
			float32_t3 spotLightSpecular =
				gSpotLights[j].color.rgb * gSpotLights[j].intencity * spotLightCalcDatas[j].spotFactor * spotLightSpecularPow * float32_t3(1.0f, 1.0f, 1.0f);
			allSpotLightSpecular += spotLightSpecular;
		}
	}

	// 全てのライトデータを入れる
	// 拡散反射+鏡面反射
	color.rgb = directionalLightDiffuse + directionalLightSpecular + allPointLightDiffuse + allPointLightSpecular + allSpotLightDiffuse + allSpotLightSpecular;
	// α
	color.a = gMaterials[instanceID].color.a * textureColor.a;

	return color;

}

PointLightCalcData CreatePointLightCalcData(VertexShaderOutput input, int index) {

	PointLightCalcData pointLightCalcData;
	pointLightCalcData.pointLightDirection = float32_t3(0.0f, 0.0f, 0.0f);
	pointLightCalcData.pointFactor = 0.0f;
	pointLightCalcData.used = true;

	if (!gPointLights[index].used) {
		pointLightCalcData.used = false;
		return pointLightCalcData;
	}

	// 逆二乗の法則
	float32_t pointDistance = length(gPointLights[index].position - input.worldPosition);
	if (pointDistance > gPointLights[index].radius) {
		pointLightCalcData.used = false;
		return pointLightCalcData;
	}
	pointLightCalcData.pointFactor = pow(saturate(-pointDistance / gPointLights[index].radius + 1.0), gPointLights[index].decay);

	pointLightCalcData.pointLightDirection = normalize(input.worldPosition - gPointLights[index].position);

	return pointLightCalcData;

}

SpotLightCalcData CreateSpotLightCalcData(VertexShaderOutput input, int index) {

	SpotLightCalcData spotLightCalcData;
	spotLightCalcData.spotLightDirectionOnSuface = float32_t3(0.0f, 0.0f, 0.0f);
	spotLightCalcData.spotFactor = 0.0f;
	spotLightCalcData.used = true;

	if (!gSpotLights[index].used) {
		spotLightCalcData.used = false;
		return spotLightCalcData;
	}

	float32_t spotDistance = length(gSpotLights[index].position - input.worldPosition);
	if (spotDistance > gSpotLights[index].distance) {
		spotLightCalcData.used = false;
		return spotLightCalcData;
	}

	spotLightCalcData.spotLightDirectionOnSuface = normalize(input.worldPosition - gSpotLights[index].position);

	spotLightCalcData.spotFactor = pow(saturate(-spotDistance / gSpotLights[index].distance + 1.0), gSpotLights[index].decay);

	float32_t cosAngle = dot(spotLightCalcData.spotLightDirectionOnSuface, gSpotLights[index].direction);
	float32_t fallofFactor = 0;
	if (gSpotLights[index].cosFalloffStart == 0.0) {
		fallofFactor = saturate((cosAngle - gSpotLights[index].cosAngle) / (1.0 - gSpotLights[index].cosAngle));
	}
	else {
		fallofFactor = saturate((cosAngle - gSpotLights[index].cosAngle) / (gSpotLights[index].cosFalloffStart - gSpotLights[index].cosAngle));
	}
	spotLightCalcData.spotFactor = spotLightCalcData.spotFactor * fallofFactor;

	return spotLightCalcData;

}

float32_t4 SetTextureColor(VertexShaderOutput input, uint32_t instanceID) {

	float32_t2 texcoord;
	float32_t4 transformedUV;
	float32_t4 textureColor;

	if (input.texcoord.x <= 1.0f) {
		texcoord = input.texcoord;
		transformedUV = mul(float32_t4(texcoord, 0.0f, 1.0f), gMaterials[instanceID].uvTransform);
		textureColor = gTexture0.Sample(gSampler, transformedUV.xy);
	}

	else if (input.texcoord.x <= 3.0f) {
		texcoord.x = input.texcoord.x - 2.0f;
		texcoord.y = input.texcoord.y;
		transformedUV = mul(float32_t4(texcoord, 0.0f, 1.0f), gMaterials[instanceID].uvTransform);
		textureColor = gTexture1.Sample(gSampler, transformedUV.xy);
	}

	else if (input.texcoord.x <= 5.0f) {
		texcoord = input.texcoord.x - 4.0f;
		texcoord.y = input.texcoord.y;
		transformedUV = mul(float32_t4(texcoord, 0.0f, 1.0f), gMaterials[instanceID].uvTransform);
		textureColor = gTexture2.Sample(gSampler, transformedUV.xy);
	}

	else if (input.texcoord.x <= 7.0f) {
		texcoord.x = input.texcoord.x - 6.0f;
		texcoord.y = input.texcoord.y;
		transformedUV = mul(float32_t4(texcoord, 0.0f, 1.0f), gMaterials[instanceID].uvTransform);
		textureColor = gTexture3.Sample(gSampler, transformedUV.xy);
	}

	else if (input.texcoord.x <= 9.0f) {
		texcoord = input.texcoord.x - 8.0f;
		texcoord.y = input.texcoord.y;
		transformedUV = mul(float32_t4(texcoord, 0.0f, 1.0f), gMaterials[instanceID].uvTransform);
		textureColor = gTexture4.Sample(gSampler, transformedUV.xy);
	}

	else if (input.texcoord.x <= 11.0f) {
		texcoord.x = input.texcoord.x - 10.0f;
		texcoord.y = input.texcoord.y;
		transformedUV = mul(float32_t4(texcoord, 0.0f, 1.0f), gMaterials[instanceID].uvTransform);
		textureColor = gTexture5.Sample(gSampler, transformedUV.xy);
	}

	else if (input.texcoord.x <= 13.0f) {
		texcoord = input.texcoord.x - 12.0f;
		texcoord.y = input.texcoord.y;
		transformedUV = mul(float32_t4(texcoord, 0.0f, 1.0f), gMaterials[instanceID].uvTransform);
		textureColor = gTexture6.Sample(gSampler, transformedUV.xy);
	}

	else {
		texcoord = input.texcoord.x - 14.0f;
		texcoord.y = input.texcoord.y;
		transformedUV = mul(float32_t4(texcoord, 0.0f, 1.0f), gMaterials[instanceID].uvTransform);
		textureColor = gTexture7.Sample(gSampler, transformedUV.xy);
	}

	return textureColor;

}

float32_t4 Enviroment(VertexShaderOutput input) {

	float32_t3 cameraToPosition = normalize(input.worldPosition - gCamera.worldPosition);
	float32_t3 reflectedVector = reflect(cameraToPosition, normalize(input.normal));
	float32_t4 enviromentColor = gEnvironmentTexture.Sample(gSampler, reflectedVector);

	return enviromentColor;
}


PixelShaderOutput main(VertexShaderOutput input) {
	PixelShaderOutput output;

	uint32_t instanceID = input.instanceId;

	float32_t4 textureColor = SetTextureColor(input, instanceID);

	float32_t3 toEye = normalize(gCamera.worldPosition - input.worldPosition);

	// ポイントライト
	PointLightCalcData pointLightCalcDatas[4];
	for (int i = 0; i < 4; i++) {
		pointLightCalcDatas[i] = CreatePointLightCalcData(input, i);
	}

	// スポットライト
	SpotLightCalcData spotLightCalcDatas[4];
	for (int j = 0; j < 4; j++) {
		spotLightCalcDatas[j] = CreateSpotLightCalcData(input, j);
	}

	// ライティング無し
	if (gMaterials[instanceID].enableLighting == 0) {
		output.color = gMaterials[instanceID].color * textureColor;
	}
	// ランバート
	else if (gMaterials[instanceID].enableLighting == 1) {
		output.color = Lambert(input, textureColor, pointLightCalcDatas, spotLightCalcDatas, instanceID);
	}
	// ハーフランバート
	else if (gMaterials[instanceID].enableLighting == 2) {
		output.color = HalfLambert(input, textureColor, pointLightCalcDatas, spotLightCalcDatas, instanceID);
	}
	// 鏡面反射
	else if (gMaterials[instanceID].enableLighting == 3) {
		output.color = PhongReflection(input, textureColor, toEye, pointLightCalcDatas, spotLightCalcDatas, instanceID);
	}
	// ブリン鏡面反射
	else if (gMaterials[instanceID].enableLighting == 4) {
		output.color = BlinnPhongReflection(input, textureColor, toEye, pointLightCalcDatas, spotLightCalcDatas, instanceID);
	}
	// その他の数が入ってきた場合
	else {
		output.color = gMaterials[instanceID].color * textureColor;
	}

	// 霧処理
	float32_t d = distance(input.worldPosition, gCamera.worldPosition);
	float32_t fogT = (gFog.fagFar - d) / (gFog.fagFar - gFog.fagNear);
	fogT = clamp(fogT, 0.0f, 1.0f);

	output.color.xyz = output.color.xyz * fogT + gFog.color.xyz * (1.0f - fogT);
	// 環境マップ
	output.color.xyz += Enviroment(input).xyz * gMaterials[instanceID].environmentCoefficient;

	// textureかoutput.colorのα値が0の時にPixelを棄却
	if (textureColor.a == 0.0 || output.color.a == 0.0) {
		discard;
	}

	output.normal.xyz = (input.normal + float32_t3(1.0f,1.0f,1.0f)) * 0.5f ;
	output.normal.a = 1.0f;

	return output;
}