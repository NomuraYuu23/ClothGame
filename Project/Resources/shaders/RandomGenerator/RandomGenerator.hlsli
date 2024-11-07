class RandomGenerator
{

	float32_t3 seed;

	float32_t rand3dTo1d(
		float32_t3 value,
		float32_t3 dotDir = float32_t3(12.9898, 78.233, 37.719)) {

		float32_t3 smallValue = sin(value);

		float32_t random = dot(smallValue, dotDir);

		random = frac(sin(random) * 143758.5453);

		return random;

	}

	float32_t2 rand3dTo2d(float32_t3 value) {

		return float32_t2(
			rand3dTo1d(value, float32_t3(12.989, 78.233, 37.719)),
			rand3dTo1d(value, float32_t3(39.346, 11.135, 83.155))
			);

	}

	float32_t3 rand3dTo3d(float32_t3 value) {

		return float32_t3(
			rand3dTo1d(value, float32_t3(12.989, 78.233, 37.719)),
			rand3dTo1d(value, float32_t3(39.346, 11.135, 83.155)),
			rand3dTo1d(value, float32_t3(73.156, 52.235, 09.151))
		);

	}

	float32_t3 Generate3d() {
		seed = rand3dTo3d(seed);
		return seed;
	}
	float32_t Generate1d() {
		float32_t result = rand3dTo1d(seed);
		seed.x = result;
		return result;
	}

};

struct PerFrame {

	float32_t time;

	float32_t deltaTime;

};