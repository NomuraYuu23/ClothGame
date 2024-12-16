#pragma once
#include "../Collider2D/ColliderShape2D.h"

/// <summary>
/// 2D衝突判定
/// </summary>
class Collision2D
{

public: // メンバ関数

	/// <summary>
	/// Box Box
	/// </summary>
	/// <param name="box1"></param>
	/// <param name="box2"></param>
	/// <returns></returns>
	static bool IsCollision(const Box& box1, const Box& box2);

	/// <summary>
	/// Circle Circle
	/// </summary>
	/// <param name="circle1"></param>
	/// <param name="circle2"></param>
	/// <returns></returns>
	static bool IsCollision(const Circle& circle1, const Circle& circle2);

	/// <summary>
	/// Segment2D Segment2D
	/// </summary>
	/// <param name="segment1"></param>
	/// <param name="segment2"></param>
	/// <returns></returns>
	static bool IsCollision(const Segment2D& segment1, const Segment2D& segment2);

	/// <summary>
	/// Box Circle
	/// </summary>
	/// <param name="box"></param>
	/// <param name="circle"></param>
	/// <returns></returns>
	static bool IsCollision(const Box& box, const Circle& circle);
	static bool IsCollision(const Circle& circle, const Box& box);

	/// <summary>
	/// Box Segment2D
	/// </summary>
	/// <param name="box"></param>
	/// <param name="segment"></param>
	/// <returns></returns>
	static bool IsCollision(const Box& box, const Segment2D& segment);
	static bool IsCollision(const Segment2D& segment, const Box& box);

	/// <summary>
	/// Circle Segment2D
	/// </summary>
	/// <param name="circle"></param>
	/// <param name="segment"></param>
	/// <returns></returns>
	static bool IsCollision(const Circle& circle, const Segment2D& segment);
	static bool IsCollision(const Segment2D& segment, const Circle& circle);

};

