#pragma once
#include "../../../Engine/3D/ManyObjects/OneOfManyObjects.h"

/// <summary>
/// 丸影
/// </summary>
class Shadow :
    public OneOfManyObjects
{

public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;
	 
	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

};

