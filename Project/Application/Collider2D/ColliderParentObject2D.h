#pragma once
#include <variant>

/// <summary>
/// 衝突時に「OnCollision」を呼ぶための設定2D版 削除可能性アリ
/// </summary>

class ParentNullObject2D;
using ColliderParentObject2D = std::variant<ParentNullObject2D*>;

/// <summary>
/// 親がないもの用
/// </summary>
class ParentNullObject2D {

};