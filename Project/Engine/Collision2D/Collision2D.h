#pragma once
#include "../Collider2D/ColliderShape2D.h"

class Collision2D
{

public: // メンバ関数

	// Box Box
	static bool IsCollision(const Box& box1, const Box& box2);

	// Circle Circle
	static bool IsCollision(const Circle& circle1, const Circle& circle2);

	// Segment2D Segment2D
	static bool IsCollision(const Segment2D& segment1, const Segment2D& segment2);

	// Box Circle
	static bool IsCollision(const Box& box, const Circle& circle);
	static bool IsCollision(const Circle& circle, const Box& box);

	// Box Segment2D
	static bool IsCollision(const Box& box, const Segment2D& segment);
	static bool IsCollision(const Segment2D& segment, const Box& box);

	// Circle Segment2D
	static bool IsCollision(const Circle& circle, const Segment2D& segment);
	static bool IsCollision(const Segment2D& segment, const Circle& circle);

};

