float32_t Gauss(in const float32_t i, in const float32_t sigma) {

	return rcp(2.0f * PI * sigma * sigma) * exp(-(i * i) * rcp(2.0f * sigma * sigma));

}

float32_t Noise(in const float32_t2 coord)
{

	return frac(sin(dot(coord, float32_t2(8.7819f, 3.255f))) * 437.645);

}

float32_t2 GetTexcoord(in const float32_t2 index, in const float32_t2 textureSize) {

	return float32_t2(
		index.x * rcp(textureSize.x),
		index.y * rcp(textureSize.y));

}

float32_t Luminance(float32_t3 v) {
	return dot(v, float32_t3(0.2125f, 0.7154f, 0.0721f));
}

float32_t3 RGBToHSV(in const float32_t3 rgb) {

	float32_t3 hsv = { 0.0f,0.0f,0.0f };

	float32_t min = rgb.r;
	float32_t max = rgb.r;

	if (min > rgb.g) {
		min = rgb.g;
	}
	if (min > rgb.b) {
		min = rgb.b;
	}

	if (max < rgb.g) {
		max = rgb.g;
	}
	if (max < rgb.b) {
		max = rgb.b;
	}

	hsv.x = max - min;
	if (hsv.x > 0.0f) {
		if (max == rgb.r) {
			hsv.x = (rgb.g - rgb.b) * rcp(hsv.x);
			if (hsv.x < 0.0f) {
				hsv.x += 6.0f;
			}
		}
		else if (max == rgb.g) {
			hsv.x = 2.0f + (rgb.b - rgb.r) * rcp(hsv.x);
		}
		else {
			hsv.x = 4.0f + (rgb.r - rgb.g) * rcp(hsv.x);
		}
	}
	hsv.x *= rcp(6.0f);

	hsv.y = (max - min);
	if (max != 0.0f) {
		hsv.y *= rcp(max);
	}

	hsv.z = max;

	return hsv;

}

float32_t3 HSVToRGB(in const float32_t3 hsv) {

	float32_t3 rgb;

	rgb.r = hsv.z;
	rgb.g = hsv.z;
	rgb.b = hsv.z;

	if (hsv.y > 0.0f) {
		float32_t h = hsv.x * 6.0f;
		int32_t i = int32_t(h);
		float32_t f = h - float32_t(i);

		switch (i) {
		default:
		case 0:
			rgb.g *= 1.0f - hsv.y * (1.0f - f);
			rgb.b *= 1.0f - hsv.y;
			break;
		case 1:
			rgb.r *= 1.0f - hsv.y * f;
			rgb.b *= 1.0f - hsv.y;
			break;
		case 2:
			rgb.r *= 1.0f - hsv.y;
			rgb.b *= 1.0f - hsv.y * (1.0f - f);
			break;
		case 3:
			rgb.r *= 1.0f - hsv.y;
			rgb.g *= 1.0f - hsv.y * f;
			break;
		case 4:
			rgb.r *= 1.0f - hsv.y * (1.0f - f);
			rgb.g *= 1.0f - hsv.y;
			break;
		case 5:
			rgb.g *= 1.0f - hsv.y;
			rgb.b *= 1.0f - hsv.y * f;
			break;
		}
	}

	return rgb;

}

float32_t WrapValue(
	const in float32_t value, 
	const in float32_t minRange, 
	const in float32_t maxRange) {

	float32_t range = maxRange - minRange;

	float32_t modValue = fmod(value - minRange, range);

	if (modValue < 0) {
		modValue += range;
	}

	return minRange + modValue;

}

float32_t4 BlurAdd(in const float32_t4 input0, in const float32_t4 input1) {

	float32_t alphaSum = input0.a + input1.a;

	if (alphaSum != 0.0f) {
		float32_t a1 = input0.a * rcp(alphaSum);
		float32_t a2 = input1.a * rcp(alphaSum);

		float32_t3 col = input0.rgb * a1 + input1.rgb * a2;

		return float32_t4(col, min(alphaSum, 1.0f));
	}

	return float32_t4(0.0f, 0.0f, 0.0f, 0.0f);

}

