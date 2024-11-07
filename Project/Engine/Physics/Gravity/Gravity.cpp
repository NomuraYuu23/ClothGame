#include "Gravity.h"

// 向き
Vector3 Gravity::dirction_ = { 0.0f, -1.0f, 0.0f };

// 力
float Gravity::power_ = 9.8f;

Vector3 Gravity::Execute()
{
    return dirction_ * (power_ * kDeltaTime_);
}
