#pragma once
#include "Vector/Vector2.h"
#include "Vector/Vector3.h"
class Ease
{

public: // サブクラス

	enum class EaseName
	{

		EaseInSine,			// 0
		EaseOutSine,		// 1
		EaseInOutSine,		// 2
		EaseInQuad,			// 3
		EaseOutQuad,		// 4
		EaseInOutQuad,		// 5
		EaseInCubic,		// 6
		EaseOutCubic,		// 7
		EaseInOutCubic,		// 8
		EaseInQuart,		// 9
		EaseOutQuart,		// 10
		EaseInOutQuart,		// 11
		EaseInQuint,		// 12
		EaseOutQuint,		// 13
		EaseInOutQuint,		// 14
		EaseInExpo,			// 15
		EaseOutExpo,		// 16
		EaseInOutExpo,		// 17
		EaseInCirc,			// 18
		EaseOutCirc,		// 19
		EaseInOutCirc,		// 20
		EaseInBack,			// 21
		EaseOutBack,		// 22
		EaseInOutBack,		// 23
		EaseInElastic,		// 24
		EaseOutElastic,		// 25
		EaseInOutElastic,	// 26
		EaseInBounce,		// 27
		EaseOutBounce,		// 28
		EaseInOutBounce,	// 29
		Lerp				// 30

	};

private: //easeTがかえってくる

	//Sine
	static float EaseInSine(float t);
	static float EaseOutSine(float t);
	static float EaseInOutSine(float t);

	//Quad
	static float EaseInQuad(float t);
	static float EaseOutQuad(float t);
	static float EaseInOutQuad(float t);

	//Cubic
	static float EaseInCubic(float t);
	static float EaseOutCubic(float t);
	static float EaseInOutCubic(float t);

	//Quart
	static float EaseInQuart(float t);
	static float EaseOutQuart(float t);
	static float EaseInOutQuart(float t);

	//Quint
	static float EaseInQuint(float t);
	static float EaseOutQuint(float t);
	static float EaseInOutQuint(float t);

	//Expo
	static float EaseInExpo(float t);
	static float EaseOutExpo(float t);
	static float EaseInOutExpo(float t);

	//Circ
	static float EaseInCirc(float t);
	static float EaseOutCirc(float t);
	static float EaseInOutCirc(float t);

	//Back
	static float EaseInBack(float t);
	static float EaseOutBack(float t);
	static float EaseInOutBack(float t);

	//Elastic
	static float EaseInElastic(float t);
	static float EaseOutElastic(float t);
	static float EaseInOutElastic(float t);

	//Bounce
	static float EaseInBounce(float t);
	static float EaseOutBounce(float t);
	static float EaseInOutBounce(float t);

	static float SetEaseT(Ease::EaseName name, float t);

public:

	//float
	static float Easing(Ease::EaseName name, const float start, const float end, float t);
	//Vector2
	static Vector2 Easing(Ease::EaseName name, const Vector2& start, const Vector2& end, float t);
	//Vector3
	static Vector3 Easing(Ease::EaseName name, const Vector3& start, const Vector3& end, float t);

};

