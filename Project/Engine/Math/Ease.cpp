#include "Ease.h"
#include "Math.h"

float Ease::EaseInSine(float t) {

	return 1.0f - cosf((t * Math::PI) / 2.0f);

}

float Ease::EaseOutSine(float t) {

	return sinf((t * Math::PI) / 2.0f);

}

float Ease::EaseInOutSine(float t) {

	return -(cosf(t * Math::PI) - 1.0f) / 2.0f;

}

//Quad�n
float Ease::EaseInQuad(float t) {

	return t * t;

}

float Ease::EaseOutQuad(float t) {

	return 1.0f - (1.0f - t) * (1.0f - t);

}

float Ease::EaseInOutQuad(float t) {

	if (t < 0.5f) {
		return 2.0f * t * t;
	}
	else {
		return 1.0f - powf(-2.0f * t + 2.0f, 2.0f) / 2.0f;
	}

}

//Cubic�n
float Ease::EaseInCubic(float t) {

	return t * t * t;

}

float Ease::EaseOutCubic(float t) {

	return 1.0f - powf(1.0f - t, 3.0f);

}

float Ease::EaseInOutCubic(float t) {

	if (t < 0.5f) {
		return 4.0f * t * t * t;
	}
	else {
		return 1.0f - powf(-2.0f * t + 2.0f, 3.0f) / 2.0f;
	}

}

//Quart�n
float Ease::EaseInQuart(float t) {

	return t * t * t * t;

}

float Ease::EaseOutQuart(float t) {

	return 1.0f - powf(1.0f - t, 4.0f);

}

float Ease::EaseInOutQuart(float t) {

	if (t < 0.5f) {
		return 8.0f * t * t * t * t;
	}
	else {
		return 1.0f - powf(-2.0f * t + 2.0f, 4.0f) / 2.0f;
	}

}

//Quint�n
float Ease::EaseInQuint(float t) {

	return t * t * t * t * t;

}

float Ease::EaseOutQuint(float t) {

	return 1.0f - powf(1.0f - t, 5.0f);

}

float Ease::EaseInOutQuint(float t) {

	if (t < 0.5f) {
		return 16.0f * t * t * t * t;
	}
	else {
		return 1.0f - powf(-2.0f * t + 2.0f, 5.0f) / 2.0f;
	}

}

//Expo�n
float Ease::EaseInExpo(float t) {

	if (t == 0.0f) {
		return 0.0f;
	}
	else {
		return powf(2.0f, 10.0f * t - 10.0f);
	}

}

float Ease::EaseOutExpo(float t) {

	if (t == 1.0f) {
		return 1.0f;
	}
	else {
		return 1.0f - powf(2.0f, -10.0f * t);
	}

}

float Ease::EaseInOutExpo(float t) {

	if (t == 0.0f) {
		return 0.0f;
	}
	else if (t == 1.0f) {
		return 1.0f;
	}
	else if (t < 0.5f) {
		return powf(2.0f, 20.0f * t - 10.0f) / 2.0f;
	}
	else {
		return (2.0f - powf(2.0f, -20.0f * t + 10.0f)) / 2.0f;
	}

}

//Circ�n
float Ease::EaseInCirc(float t) {

	return 1.0f - sqrtf(1.0f - powf(t, 2.0f));

}

float Ease::EaseOutCirc(float t) {

	return sqrtf(1.0f - powf(t - 1.0f, 2.0f));

}

float Ease::EaseInOutCirc(float t) {

	if (t < 0.5f) {
		return (1.0f - sqrtf(1.0f - powf(2.0f * t, 2.0f))) / 2.0f;
	}
	else {
		return (sqrtf(1.0f - powf(-2.0f * t + 2.0f, 2.0f)) + 1.0f) / 2.0f;
	}

}

//Back�n
float Ease::EaseInBack(float t) {

	const float c1 = 1.70158f;
	const float c3 = c1 + 1.0f;

	return c3 * t * t * t - c1 * t * t;

}

float Ease::EaseOutBack(float t) {

	const float c1 = 1.70158f;
	const float c3 = c1 + 1.0f;

	return 1.0f + c3 * powf(t - 1.0f, 3.0f) + c1 * powf(t - 1.0f, 2.0f);

}

float Ease::EaseInOutBack(float t) {

	const float c1 = 1.70158f;
	const float c2 = c1 * 1.525f;

	if (t < 0.5f) {
		return (powf(2.0f * t, 2.0f) * ((c2 + 1.0f) * 2.0f * t - c2)) / 2.0f;
	}
	else {

		return (powf(2.0f * t - 2.0f, 2.0f) * ((c2 + 1.0f) * (2.0f * t - 2.0f) + c2) + 2.0f) / 2.0f;
	}

}

//Elastic�n
float Ease::EaseInElastic(float t) {

	const float c4 = (2.0f * Math::PI) / 3.0f;

	if (t == 0.0f) {
		return 0.0f;
	}
	else if (t == 1.0f) {
		return 1.0f;
	}
	else {
		return -powf(2.0f, 10.0f * t - 10.0f) * sinf((t * 10.0f - 10.75f) * c4);
	}

}

float Ease::EaseOutElastic(float t) {

	const float c4 = (2.0f * Math::PI) / 3.0f;

	if (t == 0.0f) {
		return 0.0f;
	}
	else if (t == 1.0f) {
		return 1.0f;
	}
	else {
		return powf(2.0f, -10.0f * t) * sinf((t * 10.0f - 0.75f) * c4) + 1.0f;
	}

}

float Ease::EaseInOutElastic(float t) {

	const float c5 = (2.0f * Math::PI) / 4.5f;

	if (t == 0.0f) {
		return 0.0f;
	}
	else if (t == 1.0f) {
		return 1.0f;
	}
	else if (t < 0.5f) {
		return -(powf(2.0f, 20.0f * t - 10.0f) * sinf((20.0f * t - 11.125f) * c5)) / 2.0f;
	}
	else {
		return (powf(2.0f, -20.0f * t + 10.0f) * sinf((20.0f * t - 11.125f) * c5)) / 2.0f + 1.0f;
	}

}

//Bounce�n
float Ease::EaseInBounce(float t) {

	return 1.0f - EaseOutBounce(1.0f - t);

}

float Ease::EaseOutBounce(float t) {

	const float n1 = 7.5625f;
	const float d1 = 2.75f;

	if (t < 1.0f / d1) {
		return n1 * t * t;
	}
	else if (t < 2.0f / d1) {
		return n1 * (t -= 1.5f / d1) * t + 0.75f;
	}
	else if (t < 2.5f / d1) {
		return n1 * (t -= 2.25f / d1) * t + 0.9375f;
	}
	else {
		return n1 * (t -= 2.625f / d1) * t + 0.984375f;
	}

}

float Ease::EaseInOutBounce(float t) {

	if (t < 0.5f) {
		return (1.0f - EaseOutBounce(1.0f - 2.0f * t)) / 2.0f;
	}
	else {
		return (1.0f + EaseOutBounce(2.0f * t - 1.0f)) / 2.0f;
	}

}

float Ease::SetEaseT(Ease::EaseName name, float t)
{

	switch (name)
	{
	case Ease::EaseName::EaseInSine:
		return EaseInSine(t);
		break;
	case Ease::EaseName::EaseOutSine:
		return EaseOutSine(t);
		break;
	case Ease::EaseName::EaseInOutSine:
		return EaseInOutSine(t);
		break;
	case Ease::EaseName::EaseInQuad:
		return EaseInQuad(t);
		break;
	case Ease::EaseName::EaseOutQuad:
		return EaseOutQuad(t);
		break;
	case Ease::EaseName::EaseInOutQuad:
		return EaseInOutQuad(t);
		break;
	case Ease::EaseName::EaseInCubic:
		return EaseInCubic(t);
		break;
	case Ease::EaseName::EaseOutCubic:
		return EaseOutCubic(t);
		break;
	case Ease::EaseName::EaseInOutCubic:
		return EaseInOutCubic(t);
		break;
	case Ease::EaseName::EaseInQuart:
		return EaseInQuart(t);
		break;
	case Ease::EaseName::EaseOutQuart:
		return EaseOutQuart(t);
		break;
	case Ease::EaseName::EaseInOutQuart:
		return EaseInOutQuart(t);
		break;
	case Ease::EaseName::EaseInQuint:
		return EaseInQuint(t);
		break;
	case Ease::EaseName::EaseOutQuint:
		return EaseOutQuint(t);
		break;
	case Ease::EaseName::EaseInOutQuint:
		return EaseInOutQuint(t);
		break;
	case Ease::EaseName::EaseInExpo:
		return EaseInExpo(t);
		break;
	case Ease::EaseName::EaseOutExpo:
		return EaseOutExpo(t);
		break;
	case Ease::EaseName::EaseInOutExpo:
		return EaseInOutExpo(t);
		break;
	case Ease::EaseName::EaseInCirc:
		return EaseInCirc(t);
		break;
	case Ease::EaseName::EaseOutCirc:
		return EaseOutCirc(t);
		break;
	case Ease::EaseName::EaseInOutCirc:
		return EaseInOutCirc(t);
		break;
	case Ease::EaseName::EaseInBack:
		return EaseInBack(t);
		break;
	case Ease::EaseName::EaseOutBack:
		return EaseOutBack(t);
		break;
	case Ease::EaseName::EaseInOutBack:
		return EaseInOutBack(t);
		break;
	case Ease::EaseName::EaseInElastic:
		return EaseInElastic(t);
		break;
	case Ease::EaseName::EaseOutElastic:
		return EaseOutElastic(t);
		break;
	case Ease::EaseName::EaseInOutElastic:
		return EaseInOutElastic(t);
		break;
	case Ease::EaseName::EaseInBounce:
		return EaseInBounce(t);
		break;
	case Ease::EaseName::EaseOutBounce:
		return EaseOutBounce(t);
		break;
	case Ease::EaseName::EaseInOutBounce:
		return EaseInOutBounce(t);
		break;
	case Ease::EaseName::Lerp:
		return t;
		break;
	default:
		break;
	}

	return 0.0f;

}


float Ease::Easing(Ease::EaseName name, const float start, const float end, float t)
{

	float easeT = SetEaseT(name, t);

	return 	(1.0f - easeT) * start + easeT * end;
}

Vector2 Ease::Easing(Ease::EaseName name, const Vector2& start, const Vector2& end, float t)
{

	float easeT = SetEaseT(name, t);

	Vector2 result;
	result.x = (1.0f - easeT) * start.x + easeT * end.x;
	result.y = (1.0f - easeT) * start.y + easeT * end.y;

	return result;

}

Vector3 Ease::Easing(Ease::EaseName name, const Vector3& start, const Vector3& end, float t)
{

	float easeT = SetEaseT(name, t);

	Vector3 result;
	result.x = (1.0f - easeT) * start.x + easeT * end.x;
	result.y = (1.0f - easeT) * start.y + easeT * end.y;
	result.z = (1.0f - easeT) * start.z + easeT * end.z;

	return result;

}
