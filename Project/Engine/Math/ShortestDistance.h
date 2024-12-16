#pragma once
#include "Vector/Vector3.h"
#include "../Line/Line.h"
#include "../Line/Segment.h"

/// <summary>
/// 最短距離
/// </summary>
class ShortestDistance
{

public: // メンバ関数

    /// <summary>
    /// シングルトンインスタンスの取得
    /// </summary>
    /// <returns></returns>
    static ShortestDistance* GetInstance();

    /// <summary>
    /// 点と直線の距離
    /// </summary>
    /// <param name="p">点</param>
    /// <param name="l">直線</param>
    /// <param name="h">点から下した垂線の足(戻り値)</param>
    /// <param name="t">ベクトル係数(戻り値)</param>
    /// <returns>最短距離</returns>
    static float PointLineDist(const Vector3& p, const Line& l, Vector3& h, float& t);

    /// <summary>
    /// 点と線分の距離
    /// </summary>
    /// <param name="p">点</param>
    /// <param name="seg">線分</param>
    /// <param name="h">最短距離となる端点(戻り値)</param>
    /// <param name="t">端点位置(戻り値)</param>
    /// <returns>最短距離</returns>
    static float PointSegmentDist(const Vector3& p, const Segment& seg, Vector3& h, float& t);

    /// <summary>
    /// 2直線の最短距離
    /// </summary>
    /// <param name="l1">直線1</param>
    /// <param name="l2">直線2</param>
    /// <param name="p1">l1の垂線の足</param>
    /// <param name="p2">l2の垂線の足</param>
    /// <param name="t1">l1のベクトル係数</param>
    /// <param name="t2">l2のベクトル係数</param>
    /// <returns>最短距離</returns>
    static float LineLineDist(const Line& l1, const Line& l2, Vector3& p1, Vector3& p2, float& t1, float& t2);

    /// <summary>
    /// 2線分の最短距離
    /// </summary>
    /// <param name="l1">線分1</param>
    /// <param name="l2">線分2</param>
    /// <param name="p1">l1の垂線の足</param>
    /// <param name="p2">l2の垂線の足</param>
    /// <param name="t1">l1のベクトル係数</param>
    /// <param name="t2">l2のベクトル係数</param>
    /// <returns>最短距離</returns>
    static float SegmentSegmentDist(const Segment& seg1, const Segment& seg2, Vector3& p1, Vector3& p2, float& t1, float& t2);

private: // シングルトン
    ShortestDistance() = default;
    ~ShortestDistance() = default;
    ShortestDistance(const ShortestDistance&) = delete;
    ShortestDistance& operator=(const ShortestDistance&) = delete;

};

