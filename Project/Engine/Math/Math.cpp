#include "Math.h"
#include <cmath>

const float Math::PI = 3.14f;

float Math::Lerp(float start, float end, float t)
{
	return start + end * t;
}

float Math::LerpShortAngle(float start, float end, float t)
{

	// 角度差分を求める
	float diff = end - start;

	//-2PI,+2PIに補正
	if (diff > 2.0f * PI || diff < -2.0f * PI) {
		diff = std::fmod(diff, 2.0f * PI);
	}
	if (diff > PI) {
		diff += -2.0f * PI;
	}
	else if (diff < -PI) {
		diff += 2.0f * PI;
	}

	// らーぷ
	return Lerp(start, diff, t);

}
